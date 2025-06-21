#pragma once

#include "Component.h"

// 親からの相対位置
struct RelativePosition : public ecs::Component
{
	static const ecs::ComponentType _mType;
	float offsetX;
	float offsetY;

	RelativePosition(float aOffsetX = 0.0f, float aOffsetY = 0.0f) : offsetX(aOffsetX), offsetY(aOffsetY) {}
};