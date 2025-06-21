#pragma once

#include "Component.h"


// ノートの種類
struct NoteType : public ecs::Component
{
	static const ecs::ComponentType _mType;

	E_NoteType type;

	NoteType(E_NoteType aType) : type(aType) {}
};
