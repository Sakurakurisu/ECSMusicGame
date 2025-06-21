#pragma once

#include "Component.h"

// 回転コンポーネント
struct Rotation : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float angle;

	Rotation(float aAngle) : angle(aAngle) {}
};