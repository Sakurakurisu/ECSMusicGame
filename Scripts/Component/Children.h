#pragma once

#include "Component.h"

// 子要素への参照
struct Children : public ecs::Component
{
	static const ecs::ComponentType _mType;

	ecs::Entity parentEntity;

	Children(ecs::Entity aParentEntity) : parentEntity(aParentEntity) {}
};