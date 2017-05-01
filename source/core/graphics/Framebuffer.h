#pragma once

#include "Platform.h"
#include "GType.h"

namespace gl
{
	struct Framebuffer
	{
		int width, height;
		Guint FBO;
		Guint RBO;
		Guint colorTexture;
	};
}