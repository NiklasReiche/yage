#pragma once

#include "../TextureCreator.h"

#include "BaseObject.h"

namespace opengl
{
	class TextureCreator final : public BaseObject, public gl::ITextureCreator
	{
	public:
		std::unique_ptr<gl::ITexture2D> createTexture2D(
			int width,
			int height,
			gl::ImageFormat textureFormat,
		    const std::span<unsigned char>& data) override;

        std::unique_ptr<gl::ITexture2D> createTexture2D(
                int width,
                int height,
                gl::ImageFormat textureFormat,
		    const std::span<unsigned char>& data,
                gl::PixelTransferParams params) override;

		std::unique_ptr<gl::ICubemap> createCubemap(
			int width,
			int height,
			gl::ImageFormat textureFormat,
			const std::span<std::span<unsigned char>, 6> &data) override;

		std::unique_ptr<gl::ICubemap> createCubemap(
			int width,
			int height,
			gl::ImageFormat textureFormat,
			const std::span<std::span<unsigned char>, 6> &data,
			gl::PixelTransferParams params) override;

	private:
		using BaseObject::BaseObject;

		friend class Context;
	};
}
