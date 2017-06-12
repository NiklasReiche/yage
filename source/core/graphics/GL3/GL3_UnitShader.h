#pragma once

#include <string>
#include <vector>

#include "gl3.h"
#include "GL3_Enum.h"

namespace gl3
{
	struct GL3_UnitShaderTemplate
	{
		std::vector<GLfloat> vertices = {
			-1.0f,	1.0f,	0.0f, 1.0f,	// Top Left 
			-1.0f, -1.0f,	0.0f, 0.0f,	// Bottom Left
			1.0f, -1.0f,	1.0f, 0.0f,	// Bottom Right

			1.0f, -1.0f,	1.0f, 0.0f,	// Bottom Right
			1.0f,  1.0f,	1.0f, 1.0f,	// Top Right
			-1.0f,  1.0f,	0.0f, 1.0f	// Top Left 
		};
		std::string vertexCode =
			"	#version 330 core											\n"
			"	layout(location = 0) in vec2 position;						\n"
			"	layout(location = 1) in vec2 texCoords;						\n"
			"	out vec2 TexCoords;											\n"
			"	void main() {												\n"
			"		gl_Position = vec4(position.x, position.y, 0.0f, 1.0f); \n"
			"		TexCoords = vec2(texCoords.x, texCoords.y);				\n"
			"	}															\n";
		std::string fragmentCode =
			"	#version 330 core											\n"
			"	in vec2 TexCoords;											\n"
			"	out vec4 color;												\n"
			"	uniform sampler2D screenTexture;							\n"
			"	void main() {												\n"
			"		color = vec4(texture(screenTexture, TexCoords));		\n"
			"	}															\n";
		std::vector<int> vertexLayout = { 2, 2 };
		DrawMode mode = DrawMode::DRAW_STATIC;
	};
}