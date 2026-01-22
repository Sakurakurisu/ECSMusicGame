#pragma once

#include "System.h"

class SystemTimeAccumulator : public ecs::System
{
public:
	SystemTimeAccumulator(ecs::World& aManager) : ecs::System(aManager)
	{
		ecs::ComponentTypeSet requiredComponents;
		requiredComponents.insert(Timer::_mType);
		setRequiredComponents(requiredComponents);
	}

	virtual void updateEntity(float aDeltaTime, ecs::Entity aEntity)
	{
		Timer& timer = mWorld.getComponentStore<Timer>().get(aEntity);
		timer.accumulatedTime += aDeltaTime;
	}
};