#pragma once

#include "Component.h"

// 色コンポーネント（RGBA）
struct Color : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float R;
	float G;
	float B;
	float A;

	Color(float aR, float aG, float aB, float aA) : R(aR), G(aG), B(aB), A(aA) {}
};
