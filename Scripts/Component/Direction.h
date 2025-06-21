#pragma once

#include "Component.h"
#include <cmath>

// 方向ベクトル
struct Direction : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float vx;
	float vy;

	Direction(float aVx, float aVy) : vx(aVx), vy(aVy)
	{
		float length = std::sqrt(vx * vx + vy * vy);
		if (length != 0.0f)
		{
			vx /= length;
			vy /= length;
		}
	}
};