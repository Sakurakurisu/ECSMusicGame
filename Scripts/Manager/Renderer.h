#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "BaseManager.h"

struct Float2 {
	float x, y;
};

struct Float3 {
	float x, y, z;
};

struct Float4 {
	float r, g, b, a;
};

class Renderer : public GameFramework::BaseManager<Renderer>
{
	friend class GameFramework::BaseManager<Renderer>;
public:
	GLFWwindow* initWindow(int width, int height, const char* title);
	void Init();
	void RenderShape(const Float3& position,
					 const Float3& scale,
					 float rotation,
					 const Float4& color,
					 const std::vector<GLfloat>& vertexs,
					 const std::vector<GLuint>& indices,
					 const std::vector<GLfloat>& texCoords,
					 GLuint texture = 0);
	void DrawSpriteQuad(
		float x, float y, float w, float h,
		const Float4& color,
		float u, float v, float uw, float vh,
		GLuint textureID,
		float rotationAngle);

	void CleanUp();

	GLFWwindow* GetWindow() const
	{
		return mWindow;
	}

	GLuint LoadTexture(const char* filename);

private:
	Renderer() : shaderProgram(0), VAO(0), positionVBO(0), colorVBO(0), EBO(0) {}
	~Renderer() = default;

	GLFWwindow* mWindow = nullptr;

	GLuint shaderProgram;
	GLuint VAO, positionVBO, colorVBO, EBO;

	GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);
	GLuint compileShader(GLenum type, const char* source);
	void setupVertexData();
	float projection[16];
};
