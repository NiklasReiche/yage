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
			const std::vector<unsigned char>& data,
			gl::PixelTransferParams params) override;

		std::unique_ptr<gl::ITexture3D> createTexture3D(
			const std::vector<unsigned char> & data,
			int width,
			int height,
			int depth,
			gl::ImageFormat dataFormat,
			gl::ImageFormat textureFormat,
			int rowAlignment) override;

		std::unique_ptr<gl::ICubemap> createCubemap(
			const std::array<std::vector<unsigned char>, 6> & data,
			int width,
			int height,
			gl::ImageFormat dataFormat,
			gl::ImageFormat textureFormat,
			int rowAlignment) override;

	private:
		using GL3_BaseObject::GL3_BaseObject;

		friend class GL3_Context;
	};
}
