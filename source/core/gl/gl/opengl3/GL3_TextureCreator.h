#pragma once 

#include "../TextureCreator.h"

#include "GL3_BaseObject.h"

namespace gl3
{
	class GL3_TextureCreator final : public GL3_BaseObject, public gl::ITextureCreator
	{
	public:
		std::unique_ptr<gl::ITexture2D> createTexture2D(
			int width,
			int height,
			gl::ImageFormat textureFormat,
			const std::vector<unsigned char>& data) override;

        std::unique_ptr<gl::ITexture2D> createTexture2D(
                int width,
                int height,
                gl::ImageFormat textureFormat,
                const std::vector<unsigned char>& data,
                gl::PixelTransferParams params) override;

		std::unique_ptr<gl::ICubemap> createCubemap(
			int width,
			int height,
			gl::ImageFormat textureFormat,
			const std::array<std::vector<unsigned char>, 6> &data) override;

		std::unique_ptr<gl::ICubemap> createCubemap(
			int width,
			int height,
			gl::ImageFormat textureFormat,
			const std::array<std::vector<unsigned char>, 6> &data,
			gl::PixelTransferParams params) override;

	private:
		using GL3_BaseObject::GL3_BaseObject;

		friend class GL3_Context;
	};
}
