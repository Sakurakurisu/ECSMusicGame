#pragma once

#include "Component.h"

// 軸の定義
enum class Axis
{
	X,
	Y
};

struct Deadline : public ecs::Component
{
	static const ecs::ComponentType _mType;

	Axis axis;
	float value;

	Deadline(Axis a, float v) : axis(a), value(v) {}
};