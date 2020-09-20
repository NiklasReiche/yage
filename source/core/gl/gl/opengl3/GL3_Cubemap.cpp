#include "GL3_Cubemap.h"
#include "GL3_Context.h"

namespace gl3
{
	GL3_Cubemap::GL3_Cubemap(GL3_Cubemap &&other) noexcept
		: GL3_Texture(std::move(other))
	{
		*this = std::move(other);
	}

	GL3_Cubemap &GL3_Cubemap::operator=(GL3_Cubemap &&other) noexcept
	{
		if (this != &other)
		{
			this->width = other.width;
			this->height = other.height;

			other.width = 0;
			other.height = 0;

			GL3_Texture::operator=(std::move(other));
		}

		return *this;
	}

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
