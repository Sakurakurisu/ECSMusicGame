#pragma once

#include "Component.h"
#include <vector>

// フレームアニメーション
struct FrameAnimation : public ecs::Component
{
	static const ecs::ComponentType _mType;

	std::vector<int> frameIndices;
	float frameTime;
	float elapsedTime;
	int currentFrame;
	bool loop;

	FrameAnimation(std::vector<int> aFrameIndices, float aFrameTime, bool aLoop = true)
		: frameIndices(std::move(aFrameIndices)), frameTime(aFrameTime), elapsedTime(0.0f), currentFrame(0), loop(aLoop) {}
};