#pragma once

#include "Component.h"

// 移動スピード
struct Speed : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float value;

	Speed(float aValue) : value(aValue) {}
};