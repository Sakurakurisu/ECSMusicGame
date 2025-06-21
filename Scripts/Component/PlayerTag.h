#pragma once

#include "Component.h"

// プレイヤー識別タグ
struct PlayerTag : public ecs::Component
{
	static const ecs::ComponentType _mType;
};
