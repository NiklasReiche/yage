#include "GL3_Cubemap.h"
#include "GL3_Context.h"

namespace gl3
{
	int GL3_Cubemap::getWidth() const
	{
		return width;
	}
	int GL3_Cubemap::getHeight() const
	{
		return height;
	}
	int GL3_Cubemap::getChannels() const
	{
		return nChannels;
	}
}
