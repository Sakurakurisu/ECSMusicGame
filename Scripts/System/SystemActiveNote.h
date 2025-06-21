#pragma once

#include "System.h"

class SystemActiveNote : public ecs::System
{
public:

	SystemActiveNote(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Position::_mType);
		requiredComponents.insert(NoteTag::_mType);
		requiredComponents.insert(Trigger::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity) override
	{
		const Position& pos = mWorld.getComponentStore<Position>().get(aEntity);
		const Trigger& trigger = mWorld.getComponentStore<Trigger>().get(aEntity);
		const NoteType& noteType = mWorld.getComponentStore<NoteType>().get(aEntity);

		// ノートが特定の位置（レーン）上にあるか判定し、対応する演出エンティティを有効化
		if (trigger.isTriggered)
		{
			if (pos.x == L1)
			{
				mWorld.registerEntity(activeLight_L1);
				if (noteType.type == E_NoteType::Hold)
				{
					mWorld.registerEntity(activeLine_L1);
					mWorld.registerEntity(activeSnow_L1);
				}
			}
			else if (pos.x == L2)
			{
				mWorld.registerEntity(activeLight_L2);
				if (noteType.type == E_NoteType::Hold)
				{
					mWorld.registerEntity(activeLine_L2);
					mWorld.registerEntity(activeSnow_L2);
				}
			}
			else if (pos.x == R1)
			{
				mWorld.registerEntity(activeLight_R1);
				if (noteType.type == E_NoteType::Hold)
				{
					mWorld.registerEntity(activeLine_R1);
					mWorld.registerEntity(activeSnow_R1);
				}
			}
			else if (pos.x == R2)
			{
				mWorld.registerEntity(activeLight_R2);
				if (noteType.type == E_NoteType::Hold)
				{
					mWorld.registerEntity(activeLine_R2);
					mWorld.registerEntity(activeSnow_R2);
				}
			}
			else if (pos.x == C)
			{
				mWorld.registerEntity(activeLight_C1);
				mWorld.registerEntity(activeLight_C2);
				if (noteType.type == E_NoteType::DNA)
				{
					mWorld.registerEntity(activeLine_C1);
					mWorld.registerEntity(activeSnow_C1);
					mWorld.registerEntity(activeLine_C2);
					mWorld.registerEntity(activeSnow_C2);
				}
			}
		}
	}
};