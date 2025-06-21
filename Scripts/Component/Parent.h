#pragma once

#include "Component.h"

// 親エンティティとその子のマップ
struct Parent : public ecs::Component
{
	static const ecs::ComponentType _mType;

	std::map<std::string, ecs::Entity> children;

	Parent(std::map<std::string, ecs::Entity> aChildren) : children(aChildren) {}
};
