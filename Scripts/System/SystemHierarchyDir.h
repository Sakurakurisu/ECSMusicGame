#pragma once

#include "System.h"

class SystemHierarchyDir : public ecs::System
{
public:
	SystemHierarchyDir(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Children::_mType);
		requiredComponents.insert(Direction::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		const Children& children = mWorld.getComponentStore<Children>().get(aEntity);

		const Direction& dir = mWorld.getComponentStore<Direction>().get(children.parentEntity);
		Direction& childDir = mWorld.getComponentStore<Direction>().get(aEntity);

		childDir.vy = dir.vy;
	}
};
