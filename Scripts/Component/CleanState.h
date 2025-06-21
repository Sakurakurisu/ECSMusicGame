#pragma once
#include "Component.h"

// 消去状態
struct CleanState : public ecs::Component
{
	static const ecs::ComponentType _mType;

	bool isClean;

	CleanState(bool aIsClean = false) : isClean(aIsClean) { }
};

