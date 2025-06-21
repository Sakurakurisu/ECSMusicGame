#pragma once

#include "System.h"

class SystemNoteTap : public ecs::System
{
public:
	SystemNoteTap(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(CleanState::_mType);
		requiredComponents.insert(NoteType::_mType);
		requiredComponents.insert(Trigger::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		CleanState& cleanState = mWorld.getComponentStore<CleanState>().get(aEntity);
		const NoteType& noteType = mWorld.getComponentStore<NoteType>().get(aEntity);
		const Trigger& trigger = mWorld.getComponentStore<Trigger>().get(aEntity);

		if (cleanState.isClean == true) return;

		if (noteType.type == E_NoteType::Tap && trigger.isTriggered)
		{
			cleanState.isClean = true;
		}
	}
};
