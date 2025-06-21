#pragma once

#include "Component.h"

// スケール（拡大縮小）コンポーネント
struct Scale : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float x;
	float y;
	float uniform; // 一括スケール係数

	Scale(float aX, float aY, float aUniform = 1.0f) : x(aX), y(aY), uniform(aUniform)
	{
		x *= uniform;
		y *= uniform;
	}
};
