#pragma once

#include "System.h"

class SystemHierarchyMove : public ecs::System
{
public:
	SystemHierarchyMove(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(RelativePosition::_mType);
		requiredComponents.insert(Position::_mType);
		requiredComponents.insert(Children::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		const RelativePosition& relPos = mWorld.getComponentStore<RelativePosition>().get(aEntity);
		const Children& children = mWorld.getComponentStore<Children>().get(aEntity);
		Position& childPos = mWorld.getComponentStore<Position>().get(aEntity);

		const Position& parentPos = mWorld.getComponentStore<Position>().get(children.parentEntity);
		childPos.x = parentPos.x + relPos.offsetX;
		childPos.y = parentPos.y + relPos.offsetY;
	}
};
