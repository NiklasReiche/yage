#pragma once

#include "gl3.h"
#include "GL3_Object.h"
#include "GL3_Enum.h"

namespace gl3
{
	struct GL3_Texture : public GL3_Object
	{
		int width, height;
		TextureType target;
		InternalFormat format;
		ImageFormat px_format;
		PixelType px_type;
		int rowAlignment = 4;
		int nChannels;
	};
}