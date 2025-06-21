#pragma once

#include "Component.h"

// トリガー状態
struct Trigger : public ecs::Component
{
	static const ecs::ComponentType _mType;

	bool isTriggered;

	Trigger(bool aIsTriggered = false) : isTriggered(aIsTriggered) {}
};