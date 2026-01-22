#pragma once

#include "Component.h"

// 軌道移動（回転運動など）
struct Orbit : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float orbitSpeed;  // 回転速度（度/秒）
	bool isLooping;    // ループ回転フラグ
	bool isActive;     // 有効フラグ
	float offsetX;     // 回転中心のXオフセット
	float offsetY;     // 回転中心のYオフセット

	Orbit(float aOrbitSpeed, bool aIsLooping, bool aIsActive, float aOffsetX, float aOffsetY)
		: orbitSpeed(aOrbitSpeed), isLooping(aIsLooping), isActive(aIsActive), offsetX(aOffsetX), offsetY(aOffsetY) {}
};