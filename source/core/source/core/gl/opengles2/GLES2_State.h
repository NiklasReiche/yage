#pragma once

#include "gles2.h"
#include "GLES2_Viewport.h"

namespace gles2
{
	struct GLES2_State
	{
		int window_width;
		int window_height;
		int renderTarget_width;
		int renderTarget_height;

		GLES2_Viewport viewport;
		GLuint shader = -1;
		GLuint renderTarget = -1;

		int pack_alignment = 4;
		int unpack_alignment = 4;

		bool isDepthTestEnabled = false;
		bool isStencilTestEnabled = false;
		bool isBlendingEnabled = false;
		bool isWireframeEnabled = false;
	};
}