#pragma once

#include "Component.h"

// インデックスデータ
struct Indices : public ecs::Component
{
	static const ecs::ComponentType _mType;

	std::vector<GLuint> indices;

	Indices(std::vector<GLuint> aIndices) : indices(aIndices) {}
};