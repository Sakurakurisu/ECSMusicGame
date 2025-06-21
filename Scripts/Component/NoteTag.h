#pragma once

#include "Component.h"

// ノート識別タグ
struct NoteTag : public ecs::Component
{
	static const ecs::ComponentType _mType;

	NoteTag() {}
};
