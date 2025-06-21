#pragma once

#include "Component.h"

// ホールド状態
struct HoldState : public ecs::Component
{
	static const ecs::ComponentType _mType;
	float holdTime;           // 現在のホールド時間
	float requiredHoldTime;   // 必要なホールド時間
	HoldState(float aRequiredHoldTime) : holdTime(0.0f), requiredHoldTime(aRequiredHoldTime) {}
};