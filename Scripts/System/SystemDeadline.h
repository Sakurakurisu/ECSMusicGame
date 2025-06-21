#pragma once

#include "System.h"

// デッドライン処理を行うシステム
class SystemDeadline : public ecs::System
{
public:
	SystemDeadline(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Position::_mType);
		requiredComponents.insert(Deadline::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		const Position& pos = mWorld.getComponentStore<Position>().get(aEntity);
		const Deadline& deadline = mWorld.getComponentStore<Deadline>().get(aEntity);

		// Y軸のデッドラインを超えたかどうか判定
		if (deadline.axis == Axis::Y && pos.y > deadline.value)
		{
			// 対応する座標のエンティティを解除
			if (pos.x == L1)
			{
				mWorld.unregisterEntity(activeLight_L1);
				mWorld.unregisterEntity(activeLine_L1);
				mWorld.unregisterEntity(activeSnow_L1);
			}
			else if (pos.x == L2)
			{
				mWorld.unregisterEntity(activeLight_L2);
				mWorld.unregisterEntity(activeLine_L2);
				mWorld.unregisterEntity(activeSnow_L2);
			}
			else if (pos.x == R1)
			{
				mWorld.unregisterEntity(activeLight_R1);
				mWorld.unregisterEntity(activeLine_R1);
				mWorld.unregisterEntity(activeSnow_R1);
			}
			else if (pos.x == R2)
			{
				mWorld.unregisterEntity(activeLight_R2);
				mWorld.unregisterEntity(activeLine_R2);
				mWorld.unregisterEntity(activeSnow_R2);
			}
			else if (pos.x == C)
			{
				mWorld.unregisterEntity(activeLight_C1);
				mWorld.unregisterEntity(activeLine_C1);
				mWorld.unregisterEntity(activeSnow_C1);
				mWorld.unregisterEntity(activeLight_C2);
				mWorld.unregisterEntity(activeLine_C2);
				mWorld.unregisterEntity(activeSnow_C2);
			}

			// 対象エンティティを解除
			mWorld.unregisterEntity(aEntity);

			// 子エンティティも解除
			auto& childrenComponents = mWorld.getComponentStore<Children>().getComponents();

			for (auto& [childEntity, children] : childrenComponents)
			{
				if (children.parentEntity == aEntity)
				{
					mWorld.unregisterEntity(childEntity);
				}
			}
		}

	}
};
