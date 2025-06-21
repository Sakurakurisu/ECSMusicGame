#pragma once
#include "System.h"


class SystemMove : public ecs::System
{
public:
	SystemMove(ecs::World& aWorld) : ecs::System(aWorld)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Position::_mType);
		requiredComponents.insert(Speed::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		const Speed& speed = mWorld.getComponentStore<Speed>().get(aEntity);
		Direction& dir = mWorld.getComponentStore<Direction>().get(aEntity);
		Position& pos = mWorld.getComponentStore<Position>().get(aEntity);

		// 方向ベクトルとスピードから位置を更新
		pos.x += (dir.vx) * speed.value * aDeltaTime;
		pos.y += (dir.vy) * speed.value * aDeltaTime;
	}
};
