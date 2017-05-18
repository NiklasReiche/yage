#pragma once

#include "gl3.h"
#include "GL3_Viewport.h"

namespace gl3
{
	struct GL3_State
	{
		int window_width;
		int window_height;
		int renderTarget_width;
		int renderTarget_height;

		Viewport viewport;
		GLuint shader = -1;
		GLuint renderTarget = -1;
		GLuint VAO = 0;

		int pack_alignment = 4;
		int unpack_alignment = 4;

		bool isDepthTestEnabled = false;
		bool isStencilTestEnabled = false;
		bool isBlendingEnabled = false;
		bool isWireframeEnabled = false;
	};
}