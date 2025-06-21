#pragma once

#include "Component.h"

// タイマー
struct Timer : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float accumulatedTime;

	Timer(float aAccumulatedTime = 0) : accumulatedTime(aAccumulatedTime) {}
};