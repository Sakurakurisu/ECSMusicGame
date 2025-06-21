#pragma once

#include "Component.h"

// テクスチャ座標
struct TexCoords : public ecs::Component
{
	static const ecs::ComponentType _mType;
	std::vector<GLfloat> coords; // UV
	float uvOffset;

	TexCoords(const std::vector<GLfloat>& aCoords) : coords(aCoords), uvOffset(0.0f) {}
};