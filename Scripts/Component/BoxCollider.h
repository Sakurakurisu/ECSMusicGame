#pragma once

#include "Component.h"

// 衝突判定用の矩形
struct BoxCollider : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float offsetX;
	float offsetY;
	float width;
	float height;

	BoxCollider(float aOffsetX, float aOffsetY, float aWidth, float aHeight)
		: offsetX(aOffsetX), offsetY(aOffsetY), width(aWidth), height(aHeight) {}
};