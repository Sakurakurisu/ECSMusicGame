#pragma once

#include "Component.h"

// 当たり判定領域
struct Area : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float left;
	float right;
	float top;
	float bottom;

	Area(float aLeft, float aRight, float aTop, float aDown)
		: left(aLeft), right(aRight), top(aTop), bottom(aDown) {}
};