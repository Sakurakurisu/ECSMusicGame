#pragma once

#include "Component.h"

// 軌道移動（回転運動など）
struct Orbit : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float orbitSpeed;  // 旋转速度（度/秒）
	bool isLooping;    // 是否循环旋转
	bool isActive;     // 是否激活旋转
	float offsetX;     // 相对于原点的旋转中心X轴偏移
	float offsetY;     // 相对于原点的旋转中心Y轴偏移

	Orbit(float aOrbitSpeed, bool aIsLooping, bool aIsActive, float aOffsetX, float aOffsetY)
		: orbitSpeed(aOrbitSpeed), isLooping(aIsLooping), isActive(aIsActive), offsetX(aOffsetX), offsetY(aOffsetY) {}
};