#pragma once

#include "Component.h"

// 位置コンポーネント
struct Position : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float x;
	float y;

	Position(float aX, float aY) : x(aX), y(aY) {}
};