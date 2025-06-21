#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Stb/stb_image.h"

const char* VertexShaderSource =
"#version 450\n"
"precision highp float;\n"

"uniform mat4 uProjection;\n"

"layout( location = 0 ) in vec3 inPosition;\n"
"layout( location = 1 ) in vec4 inColor;\n"
"layout( location = 2 ) in vec2 inTexCoord;\n"

"out vec4 vColor;\n"
"out vec2 vTexCoord;\n"

"void main() {\n"
"    vColor = inColor;\n"
"    vTexCoord = inTexCoord;\n"
"    gl_Position = vec4(inPosition, 1.0) * uProjection;\n"
"}\n";

const char* FragmentShaderSource =
"#version 450\n"
"precision highp float;\n"

"uniform sampler2D uSampler;\n"
"uniform bool uTextureEnable;\n"

"in vec4 vColor;\n"
"in vec2 vTexCoord;\n"

"out vec4 outColor;\n"

"void main() {\n"
"    if(uTextureEnable)\n"
"		outColor = vColor * texture(uSampler, vTexCoord);\n"
"    else\n"
"		outColor = vColor;\n"
"}\n";

GLFWwindow* Renderer::initWindow(int width, int height, const char* title) {
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return nullptr;
	}

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return nullptr;
	}

	mWindow = window;
	return window;
}

void Renderer::Init() {
	// シェーダープログラムを初期化
	shaderProgram = createShaderProgram(VertexShaderSource, FragmentShaderSource);

	// 正射影行列を手動で生成
	float left = -960.0f, right = 960.0f;
	float bottom = -540.0f, top = 540.0f;
	float near = -1.0f, far = 1.0f;

	// 単位行列を初期化し、メンバ変数 `projection` に代入
	projection[0] = 2.0f / (right - left); projection[1] = 0;                   projection[2] = 0;                    projection[3] = 0;
	projection[4] = 0;                    projection[5] = 2.0f / (top - bottom); projection[6] = 0;                    projection[7] = 0;
	projection[8] = 0;                    projection[9] = 0;                    projection[10] = -2.0f / (far - near); projection[11] = 0;
	projection[12] = -(right + left) / (right - left);
	projection[13] = -(top + bottom) / (top - bottom);
	projection[14] = -(far + near) / (far - near);
	projection[15] = 1;

	// 投影行列をシェーダーに渡す
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 頂点データとバッファを設定
	setupVertexData();
}


void Renderer::RenderShape(const Float3& position,
						   const Float3& scale,
						   float rotation,
						   const Float4& color,
						   const std::vector<GLfloat>& vertexs,
						   const std::vector<GLuint>& indices,
						   const std::vector<GLfloat>& texCoords,
						   GLuint texture)
{
	// モデル行列を作成
	float cosTheta = std::cos(rotation);
	float sinTheta = std::sin(rotation);

	// 単位行列を初期化
	float model[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	// 平行移動を適用
	model[12] = position.x;
	model[13] = position.y;
	model[14] = position.z;

	// 回転を適用（Z軸回り）
	model[0] = cosTheta * scale.x;
	model[1] = sinTheta * scale.x;
	model[4] = -sinTheta * scale.y;
	model[5] = cosTheta * scale.y;

	// 拡大縮小を適用
	model[10] = scale.z;

	// VAO をバインド
	glBindVertexArray(VAO);

	// 位置の VBO を更新
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(GLfloat), vertexs.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// 色の VBO を更新
	std::vector<GLfloat> colors = {
		color.r, color.g, color.b, color.a,
		color.r, color.g, color.b, color.a,
		color.r, color.g, color.b, color.a,
		color.r, color.g, color.b, color.a
	};

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	// テクスチャ座標を処理（テクスチャ使用時）
	GLuint texCoordVBO = 0;
	if (texture != 0)
	{
		glGenBuffers(1, &texCoordVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(2);
	}

	// EBO をバインド
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glUseProgram(shaderProgram);

	// モデル行列をシェーダーに設定
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);

	// テクスチャ使用フラグを設定
	GLint useTextureLoc = glGetUniformLocation(shaderProgram, "useTexture");
	glUniform1i(useTextureLoc, texture != 0); // テクスチャがある場合は 1、ない場合は 0

	if (texture != 0)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	// 図形を描画
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	// VBO をクリーンアップ（必要な場合）
	if (texCoordVBO != 0)
	{
		glDeleteBuffers(1, &texCoordVBO);
	}
}


void Renderer::CleanUp() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &positionVBO);
	glDeleteBuffers(1, &colorVBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
}

GLuint Renderer::createShaderProgram(const char* vertexSource, const char* fragmentSource) {
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLuint Renderer::compileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return shader;
}

void Renderer::setupVertexData()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &positionVBO);
	glGenBuffers(1, &colorVBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

GLuint Renderer::LoadTexture(const char* filename)
{
	stbi_set_flip_vertically_on_load(true);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format{};
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		printf("Failed to load texture: %s\n", filename);
		stbi_image_free(data);
	}

	return textureID;
}

void Renderer::DrawSpriteQuad(
	float x, float y, float w, float h,
	const Float4& color,
	float u, float v, float uw, float vh,
	GLuint textureID,
	float rotationAngle)  // 新しく追加された回転角度パラメータ（度）
{
	// 回転角度を度からラジアンに変換
	const float pi = 3.14159265358979323846f;
	float radians = rotationAngle * (pi / 180.0f);

	// 回転後の4つの頂点の位置を計算
	float cosTheta = cos(radians);
	float sinTheta = sin(radians);

	std::vector<GLfloat> vertexData = {
		// 頂点1
		x + (w * 0.5f) * cosTheta - (-h * 0.5f) * sinTheta,
		y + (w * 0.5f) * sinTheta + (-h * 0.5f) * cosTheta, 0.0f, // 頂点1の位置
		color.r, color.g, color.b, color.a,                    // 頂点1の色
		u + uw, v,                                             // 頂点1のテクスチャ座標

		// 頂点2
		x + (-w * 0.5f) * cosTheta - (-h * 0.5f) * sinTheta,
		y + (-w * 0.5f) * sinTheta + (-h * 0.5f) * cosTheta, 0.0f, // 頂点2の位置
		color.r, color.g, color.b, color.a,                    // 頂点2の色
		u, v,                                                  // 頂点2のテクスチャ座標

		// 頂点3
		x + (w * 0.5f) * cosTheta - (h * 0.5f) * sinTheta,
		y + (w * 0.5f) * sinTheta + (h * 0.5f) * cosTheta, 0.0f, // 頂点3の位置
		color.r, color.g, color.b, color.a,                    // 頂点3の色
		u + uw, v + vh,                                        // 頂点3のテクスチャ座標

		// 頂点4
		x + (-w * 0.5f) * cosTheta - (h * 0.5f) * sinTheta,
		y + (-w * 0.5f) * sinTheta + (h * 0.5f) * cosTheta, 0.0f, // 頂点4の位置
		color.r, color.g, color.b, color.a,                    // 頂点4の色
		u, v + vh                                              // 頂点4のテクスチャ座標
	};

	// 頂点バッファを更新
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(GLfloat), vertexData.data(), GL_DYNAMIC_DRAW);

	// VAO をバインドして頂点属性ポインタを設定
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// シェーダープログラムを使用
	glUseProgram(shaderProgram);

	// 投影行列を設定
	GLint projLoc = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);

	// テクスチャをバインド
	if (textureID != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		GLint useTextureLoc = glGetUniformLocation(shaderProgram, "uTextureEnable");
		glUniform1i(useTextureLoc, 1); // テクスチャを使用する
	}
	else
	{
		GLint useTextureLoc = glGetUniformLocation(shaderProgram, "uTextureEnable");
		glUniform1i(useTextureLoc, 0); // テクスチャを使用しない
	}

	// 四角形を描画
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// VAO のバインドを解除
	glBindVertexArray(0);
}
