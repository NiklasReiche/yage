#pragma once

#include "../Cubemap.h"
#include "GL3_Texture.h"

namespace gl3
{
	class GL3_Cubemap : public GL3_Texture, public gl::ICubemap, public std::enable_shared_from_this<GL3_Cubemap>
	{
	public:
		~GL3_Cubemap() override = default;

		int getWidth() const override;

		int getHeight() const override;

		int getChannels() const override;

	private:
		int width = 0;
		int height = 0;

		using GL3_Texture::GL3_Texture;

		GL3_Cubemap(GL3_Cubemap &&other) noexcept;

		GL3_Cubemap &operator=(GL3_Cubemap &&other) noexcept;

		friend class GL3_TextureCreator;
	};
}
