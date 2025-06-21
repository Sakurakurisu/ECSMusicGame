#pragma once

#include "Component.h"

// テクスチャID
struct Texture : public ecs::Component
{
	static const ecs::ComponentType _mType;

	GLuint ID;

	Texture(GLuint aID) : ID(aID) {}
};
