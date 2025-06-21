#pragma once

#include "Component.h"
#include <vector>
#include <GL\glew.h>

// 頂点データ
struct Vertex : public ecs::Component
{
	static const ecs::ComponentType _mType;

	// 順番：
	// 左下
	// 右下
	// 右上
	// 左上
	std::vector<GLfloat> vertices; // 頂点配列

	Vertex(std::vector<GLfloat> aVertices) : vertices(aVertices) {}
};