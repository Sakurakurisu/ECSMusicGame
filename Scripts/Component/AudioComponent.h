#pragma once

#include "Component.h"

// 音声コンポーネント
struct AudioComponent : public ecs::Component
{
	static const ecs::ComponentType _mType;

	std::string filePath; // 音声ファイルのパス
	float volume;         // 音量
	bool loop;            // ループ再生
	bool isPlaying;       // 再生中か

	AudioComponent(const std::string& aFilePath, float aVolume = 1.0f, bool aLoop = false)
		: filePath(aFilePath), volume(aVolume), loop(aLoop), isPlaying(false) {}
};