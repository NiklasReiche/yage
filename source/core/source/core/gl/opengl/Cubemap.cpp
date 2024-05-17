#include "Cubemap.h"
#include "Context.h"

namespace opengl
{
    Cubemap::~Cubemap()
    {
        lockContextPtr()->bindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

	Cubemap::Cubemap(Cubemap&&other) noexcept
		: Texture(std::move(other))
	{
		*this = std::move(other);
	}

	Cubemap& Cubemap::operator=(Cubemap&&other) noexcept
	{
		if (this != &other)
		{
			this->width = other.width;
			this->height = other.height;

			other.width = 0;
			other.height = 0;

			Texture::operator=(std::move(other));
		}

		return *this;
	}

	int Cubemap::getWidth() const
	{
		return width;
	}
	int Cubemap::getHeight() const
	{
		return height;
	}
	int Cubemap::getChannels() const
	{
		return nChannels;
	}
}
