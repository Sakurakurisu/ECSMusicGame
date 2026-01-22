#include "AudioManager.h"
#include <iostream>
#include <fstream>
#include <vector>

AudioManager::AudioManager()
	: device(nullptr), context(nullptr), bgmSource(0) {}

AudioManager::~AudioManager() {
	cleanup();
}

bool AudioManager::initialize() {
	device = alcOpenDevice(nullptr);
	if (!device) {
		std::cerr << "Failed to open audio device" << std::endl;
		return false;
	}

	context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context)) {
		std::cerr << "Failed to make context current" << std::endl;
		alcCloseDevice(device);
		return false;
	}

	// BGM用ソースを生成
	alGenSources(1, &bgmSource);
	if (alGetError() != AL_NO_ERROR) {
		std::cerr << "Failed to generate sources" << std::endl;
		return false;
	}

	return true;
}

void AudioManager::cleanup() {
	cleanUpBuffersAndSources();
	if (context) {
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
	}
	if (device) {
		alcCloseDevice(device);
	}
}

void AudioManager::cleanUpBuffersAndSources() {
	for (auto& pair : sources) {
		alDeleteSources(1, &pair.second);
	}
	for (auto& pair : buffers) {
		alDeleteBuffers(1, &pair.second);
	}
}

ALuint AudioManager::loadWAV(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary);
	if (!file) {
		std::cerr << "Failed to open WAV file: " << filePath << std::endl;
		return 0;
	}

	char buffer[4];

	// WAVファイルヘッダ解析：RIFF → WAVE → fmt → data の順で検証
	file.read(buffer, 4);
	if (strncmp(buffer, "RIFF", 4) != 0) {
		std::cerr << "Not a valid WAV file (missing RIFF header)" << std::endl;
		return 0;
	}

	file.ignore(4); // ファイルサイズをスキップ

	file.read(buffer, 4);
	if (strncmp(buffer, "WAVE", 4) != 0) {
		std::cerr << "Not a valid WAV file (missing WAVE header)" << std::endl;
		return 0;
	}

	file.read(buffer, 4);
	if (strncmp(buffer, "fmt ", 4) != 0) {
		std::cerr << "Not a valid WAV file (missing fmt header)" << std::endl;
		return 0;
	}

	file.ignore(4); // サブチャンクサイズをスキップ

	short audioFormat;
	file.read(reinterpret_cast<char*>(&audioFormat), sizeof(short));
	if (audioFormat != 1) { // PCMフォーマットのみ対応
		std::cerr << "Unsupported WAV file format (not PCM)" << std::endl;
		return 0;
	}

	short channels;
	file.read(reinterpret_cast<char*>(&channels), sizeof(short));

	int sampleRate;
	file.read(reinterpret_cast<char*>(&sampleRate), sizeof(int));

	file.ignore(6); // ByteRate, BlockAlignをスキップ

	short bitsPerSample;
	file.read(reinterpret_cast<char*>(&bitsPerSample), sizeof(short));

	// dataチャンクを探す（他のチャンクはスキップ）
	while (true) {
		file.read(buffer, 4);
		if (strncmp(buffer, "data", 4) == 0) {
			break;
		}
		else {
			int chunkSize;
			file.read(reinterpret_cast<char*>(&chunkSize), sizeof(int));
			file.ignore(chunkSize);
		}
	}

	int dataSize;
	file.read(reinterpret_cast<char*>(&dataSize), sizeof(int));

	std::vector<char> data(dataSize);
	file.read(data.data(), dataSize);

	// OpenALバッファを生成してデータを転送
	ALuint bufferID;
	alGenBuffers(1, &bufferID);
	ALenum format;

	if (channels == 1) {
		format = (bitsPerSample == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
	}
	else if (channels == 2) {
		format = (bitsPerSample == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
	}
	else {
		std::cerr << "Unsupported number of channels: " << channels << std::endl;
		return 0;
	}

	alBufferData(bufferID, format, data.data(), dataSize, sampleRate);
	if (alGetError() != AL_NO_ERROR) {
		std::cerr << "Error loading sound data into buffer" << std::endl;
		return 0;
	}

	//std::cout << "Channels: " << channels << std::endl;
	//std::cout << "Sample Rate: " << sampleRate << std::endl;
	//std::cout << "Bits Per Sample: " << bitsPerSample << std::endl;
	//std::cout << "Data Size: " << dataSize << std::endl;
	//std::cout << "Sound loaded successfully into buffer" << std::endl;

	return bufferID;
}

void AudioManager::loadSound(const std::string& name, const std::string& filePath) {
	ALuint buffer = loadWAV(filePath);
	if (buffer == 0) return;

	ALuint source;
	alGenSources(1, &source);
	alSourcei(source, AL_BUFFER, buffer);

	buffers[name] = buffer;
	sources[name] = source;
}

void AudioManager::playSound(const std::string& name) {
	auto it = sources.find(name);
	if (it != sources.end()) {
		alSourcePlay(it->second);
	}
	else {
		std::cerr << "Sound not found: " << name << std::endl;
	}
}

void AudioManager::stopSound(const std::string& name) {
	auto it = sources.find(name);
	if (it != sources.end()) {
		alSourceStop(it->second);
	}
	else {
		std::cerr << "Sound not found: " << name << std::endl;
	}
}

void AudioManager::playBGM(const std::string& filePath, bool loop) {
	ALuint buffer = loadWAV(filePath);
	if (buffer == 0) return;

	alSourceStop(bgmSource);
	alSourcei(bgmSource, AL_BUFFER, buffer);
	alSourcei(bgmSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	//alSourcef(bgmSource, AL_GAIN, 1.0f);
	alSourcePlay(bgmSource);
}

void AudioManager::stopBGM() {
	alSourceStop(bgmSource);
}