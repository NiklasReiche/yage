#pragma once

#include "../Cubemap.h"
#include "Texture.h"

namespace opengl
{
	class Cubemap : public Texture, public gl::ICubemap, public std::enable_shared_from_this<Cubemap>
	{
	public:
		~Cubemap() override = default;

		int getWidth() const override;

		int getHeight() const override;

		int getChannels() const override;

	private:
		int width = 0;
		int height = 0;

		using Texture::Texture;

		Cubemap(Cubemap&&other) noexcept;

		Cubemap&operator=(Cubemap&&other) noexcept;

		friend class TextureCreator;
	};
}
