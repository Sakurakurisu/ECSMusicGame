#pragma once

#include "Component.h"

// 出現間隔（秒）
struct SpawnInterval : public ecs::Component
{
	static const ecs::ComponentType _mType;

	float spawnTime;

	SpawnInterval(float aSpawnTime) : spawnTime(aSpawnTime) {}
};