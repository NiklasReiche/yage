#pragma once

#include <string>
#include <vector>

#include "gles2.h"
#include "GLES2_Enum.h"

namespace gles2
{
	struct GLES2_UnitShaderTemplate
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
			"	#version 100													\n"
			"	attribute vec2 position;										\n"
			"	attribute vec2 texCoords;										\n"
			"	varying vec2 TexCoords;											\n"
			"	void main() {													\n"
			"		gl_Position = vec4(position.x, position.y, 0.0, 1.0);		\n"
			"		TexCoords = vec2(texCoords.x, texCoords.y);					\n"
			"	}																\n";
		std::string fragmentCode =
			"	#version 100													\n"
			"	precision mediump float;										\n"
			"	varying vec2 TexCoords;											\n"
			"	uniform sampler2D screenTexture;								\n"
			"	void main() {													\n"
			"		gl_FragColor = vec4(texture2D(screenTexture, TexCoords));	\n"
			"	}																\n";
		std::vector<std::string> shaderAttributes = { "position", "texCoords" };
		std::vector<int> vertexLayout = { 2, 2 };
		DrawMode mode = DrawMode::DRAW_STATIC;
	};
}