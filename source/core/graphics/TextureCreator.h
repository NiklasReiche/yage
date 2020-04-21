#pragma once

#include <memory>
#include <vector>
#include <array>

#include "Texture2D.h"
#include "Texture3D.h"
#include "Cubemap.h"
#include "ImageFormat.h"

namespace gl
{
	class ITextureCreator
	{
	public:
		virtual ~ITextureCreator() = default;

		[[nodiscard]]
		virtual std::unique_ptr<ITexture2D> createTexture2D(
			const std::vector<unsigned char> & data,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ITexture3D> createTexture3D(
			const std::vector<unsigned char> & data,
			int width,
			int height,
			int depth,
			ImageFormat format,
			int rowAlignment = 4) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ICubemap> createCubemap(
			const std::array<std::vector<unsigned char>, 6> & data,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4) = 0;

	protected:
		ITextureCreator() = default;
		ITextureCreator(const ITextureCreator& other) = default;
		ITextureCreator(ITextureCreator&& other) = default;
		ITextureCreator& operator=(const ITextureCreator & other) = default;
		ITextureCreator& operator=(ITextureCreator&& other) = default;
	};
}
