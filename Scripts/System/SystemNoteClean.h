#pragma once

#include "System.h"
#include "Deadline.h"
#include "CleanState.h"
#include "World.h"

class SystemNoteClean : public ecs::System
{
public:
	SystemNoteClean(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Deadline::_mType);
		requiredComponents.insert(CleanState::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		Deadline& deadline = mWorld.getComponentStore<Deadline>().get(aEntity);
		const CleanState& cleanState = mWorld.getComponentStore<CleanState>().get(aEntity);
		if (cleanState.isClean)
			deadline.value = 300.0f;
	}
};