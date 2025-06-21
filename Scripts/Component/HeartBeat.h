#pragma once

#include "Component.h"

// 心拍（拡大/縮小）エフェクト
struct HeartBeat : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float growthRate;
	float maxScaleY;
	bool growing;
	bool isPaused;
	float pauseDuration;
	float currentPauseTime;
	float scrollSpeed;
	bool isActive;

	HeartBeat(float aGrowthRate, float aMaxScaleY, float aPauseDuration, float aScollSpeed)
		: growthRate(aGrowthRate), maxScaleY(aMaxScaleY), growing(true),
		isPaused(false), pauseDuration(aPauseDuration), currentPauseTime(0.0f), scrollSpeed(aScollSpeed), isActive(false) {}
};
