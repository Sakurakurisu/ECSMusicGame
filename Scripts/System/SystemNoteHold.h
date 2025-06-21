#pragma once

#include "System.h"

class SystemNoteHold : public ecs::System
{
public:
	SystemNoteHold(ecs::World& aWorld) : ecs::System(aWorld) {
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(HoldState::_mType);
		requiredComponents.insert(CleanState::_mType);
		requiredComponents.insert(NoteType::_mType);
		requiredComponents.insert(Trigger::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		HoldState& holdState = mWorld.getComponentStore<HoldState>().get(aEntity);
		CleanState& cleanState = mWorld.getComponentStore<CleanState>().get(aEntity);
		const NoteType& noteType = mWorld.getComponentStore<NoteType>().get(aEntity);
		const Trigger& trigger = mWorld.getComponentStore<Trigger>().get(aEntity);

		if (noteType.type == E_NoteType::Hold && trigger.isTriggered)
		{
			holdState.holdTime += aDeltaTime;

			if (holdState.holdTime >= holdState.requiredHoldTime)
			{
				cleanState.isClean = true;
			}
		}
		else
		{
			holdState.holdTime = 0.0f;
		}
	}
};