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

		/**
		 * @brief Creates a new 2D-texture.
		 *
		 * The texture will be created with the given dimensions and pixel format. These cannot change after the
		 * texture was created.
		 *
		 * The input data must be correctly sized, i.e.
		 * data.size = (width+pad) * height * channels,
		 * where pad is determined by the row alignment of the input data.
		 *
		 * If the input data is non-empty the texture will be initialized with the container values.
		 * If the input data is empty, an empty uninitialized texture will be created.
		 *
		 * The pixel format of the input data does not need to equal the internal format of the texture.
		 * If the formats differ, the input data will be converted to the texture's format.
		 * The texture's internal format remains unchanged.
		 * If the data format is UNDEFINED, the texture's internal format will be used for interpreting the input.
		 * 
		 * @param width Width of the texture.
		 * @param height Height of the texture.
		 * @param textureFormat Pixel format of the texture.
		 * @param data A set of sub-pixel values with which to initialize the texture.
		 * @param params Hints on how the input data is formatted.
		 * @return The initialized texture.
		 *
		 * @throws std::invalid_argument The data is not correctly sized.
		 * @throws std::invalid_argument The width or height is not strictly positive.
		 * @throws std::invalid_argument The texture format is undefined.
		 * @throws std::logic_error The graphics context of this TextureCreator was destroyed.
		 */
		[[nodiscard]]
		virtual std::unique_ptr<ITexture2D> createTexture2D(
			int width,
			int height,
			ImageFormat textureFormat,
			const std::vector<unsigned char>& data,
			PixelTransferParams params = { ImageFormat::UNDEFINED, RowAlignment::B_1 }
		) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ITexture3D> createTexture3D(
			const std::vector<unsigned char> & data,
			int width,
			int height,
			int depth,
			ImageFormat dataFormat,
			ImageFormat textureFormat,
			int rowAlignment = 1) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ICubemap> createCubemap(
			const std::array<std::vector<unsigned char>, 6> & data,
			int width,
			int height,
			ImageFormat dataFormat,
			ImageFormat textureFormat,
			int rowAlignment = 1) = 0;

	protected:
		ITextureCreator() = default;
		ITextureCreator(const ITextureCreator& other) = default;
		ITextureCreator(ITextureCreator&& other) = default;
		ITextureCreator& operator=(const ITextureCreator & other) = default;
		ITextureCreator& operator=(ITextureCreator&& other) = default;
	};
}
