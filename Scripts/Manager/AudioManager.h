#pragma once

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <string>
#include <map>

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	bool initialize();     // OpenAL を初期化する
	void cleanup();        // OpenAL をクリーンアップする

	void loadSound(const std::string& name, const std::string& filePath);
	void playSound(const std::string& name);
	void stopSound(const std::string& name);
	void playBGM(const std::string& filePath, bool loop = true);
	void stopBGM();

private:
	ALuint loadWAV(const std::string& filePath);
	void cleanUpBuffersAndSources();

	ALCdevice* device;
	ALCcontext* context;

	std::map<std::string, ALuint> buffers;
	std::map<std::string, ALuint> sources;
	ALuint bgmSource;
};
