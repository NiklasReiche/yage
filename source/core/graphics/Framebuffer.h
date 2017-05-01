#pragma once

#include "Platform.h"
#include "GType.h"
#include "math\MVector.h"

namespace gl
{
	struct Framebuffer
	{
		int width, height;
		Guint FBO;
		Guint RBO;
		Guint colorTexture;
		gml::Vec4<float> clearColor;
	};
}