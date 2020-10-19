#pragma once

#include <vector>

#include <utils/Point.h>

#include "ImageFormat.h"
#include "PixelTransfer.h"

namespace gl
{
	/**
	 * @brief Interface for two-dimensional textures.
	 */
	class ITexture2D
	{
	public:
		virtual ~ITexture2D() = default;

		/**
		 * @brief Overwrites the current image data in this texture with the given data.
		 *
		 * The input data must be correctly sized, i.e.
		 * data.size = texture.width * texture.height * texture.channels.
		 *
		 * The pixel format of the input data must equal the internal format of the texture.
		 * The input data must be tightly packed, i.e. contain no padding.
		 * To overwrite the texture with differently formatted input data the overload can be used.
		 *
		 * @param data A set of sub-pixel values.
		 *
		 * @throws std::invalid_argument The data container is not sized correctly.
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		virtual void setImage(const std::span<const unsigned char>& data) = 0;

		/**
		 * @brief Overwrites the current image data in this texture with the given data.
		 *
		 * The input data must be correctly sized, i.e.
		 * data.size = (texture.width+pad) * texture.height * texture.channels,
		 * where pad is determined by the row alignment of the input data.
		 *
		 * The pixel format of the input data does not need to equal the internal format of the texture.
		 * If the formats differ, the input data will be converted to the texture's format.
		 * The texture's internal format remains unchanged.
		 *
		 * @param data A set of sub-pixel values.
		 * @param params Hints on how the input data is formatted.
		 *
		 * @throws std::invalid_argument The data container is not sized correctly.
		 * @throws std::invalid_argument The image format is undefined.
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		virtual void setImage(const std::span<const unsigned char>& data, PixelTransferParams params) = 0;

		/**
		 * @brief Overwrites an area of the image data in this texture with the given data.
		 *
		 * The input data must be correctly sized, i.e. data.size = width * height * texture.channels.
		 *
		 * The pixel format of the input data must equal the internal format of the texture.
		 * The input data must be tightly packed, i.e. contain no padding.
		 * To overwrite the texture with differently formatted input data the overload can be used.
		 *
		 * @param subArea The area of the texture to overwrite.
		 * @param data A set of sub-pixel values.
		 *
		 * @throws std::invalid_argument The specified area is out of bounds for this texture.
		 * @throws std::invalid_argument The data container is not sized correctly.
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		virtual void setSubImage(utils::Area subArea, const std::span<const unsigned char>& data) = 0;

		/**
		 * @brief Overwrites an area of the image data in this texture with the given data.
		 *
		 * The input data must be correctly sized, i.e. data.size = (width+pad) * height * texture.channels,
		 * where pad is determined by the row alignment of the input data.
		 *
		 * The pixel format of the input data does not need to equal the internal format of the texture.
		 * If the formats differ, the input data will be converted to the texture's format.
		 * The texture's internal format remains unchanged.
		 *
		 * @param subArea The area of the texture to overwrite.
		 * @param data A set of sub-pixel values.
		 * @param params Hints on how the input data is formatted.
		 *
		 * @throws std::invalid_argument The specified area is out of bounds for this texture.
		 * @throws std::invalid_argument The data container is not sized correctly.
		 * @throws std::invalid_argument The image format is undefined.
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		virtual void setSubImage(
			utils::Area subArea,
		    const std::span<const unsigned char>& data,
			PixelTransferParams params) = 0;

		/**
		 * @brief Reads the image data from this texture.
		 *
		 * The output data will be tightly packed, i.e. contain no padding, and have the same format as the texture.
		 * To format the output data differently the overload can be used.
		 *
		 * @return A tightly-packed set of sub-pixel values.
		 *
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		[[nodiscard]]
		virtual std::vector<unsigned char> getImage() = 0;

		/**
		 * @brief Reads the image data from this texture.
		 *
		 * The pixel format of the output data does not need to equal the internal format of the texture.
		 * If the formats differ, the texture's data will be converted to the output format after it is read.
		 * The texture's internal format remains unchanged.
		 *
		 * @param params Hint on how the output data should be formatted.
		 * @return A set of sub-pixel values.
		 *
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		[[nodiscard]]
		virtual std::vector<unsigned char> getImage(PixelTransferParams params) = 0;

		/**
		 * @brief Reads the image data from a mipmap level of this texture.
		 *
		 * Mipmaps can only be read if they were previously generated for this texture.
		 *
		 * The output data will be tightly packed, i.e. contain no padding, and have the same format as the texture.
		 * To format the output data differently the overload can be used.
		 *
		 * @param level The mipmap level to read.
		 * @return A tightly-packed set of sub-pixel values.
		 *
		 * @throws std::invalid_argument There are no mipmaps for the given level.
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		[[nodiscard]]
		virtual std::vector<unsigned char> getMipmapImage(int level) = 0;

		/**
		 * @brief Reads the image data from a mipmap level of this texture.
		 *
		 * Mipmaps can only be read if they were previously generated for this texture.
		 *
		 * The pixel format of the output data does not need to equal the internal format of the texture.
		 * If the formats differ, the texture's data will be converted to the output format after it is read.
		 * The texture's internal format remains unchanged.
		 * If the data format is UNDEFINED, the texture's internal format will be used for interpreting the input.
		 *
		 * @param level The mipmap level to read.
		 * @param params Hint on how the output data should be formatted.
		 * @return A set of sub-pixel values.
		 *
		 * @throws std::invalid_argument There are no mipmaps for the given level.
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		[[nodiscard]]
		virtual std::vector<unsigned char> getMipmapImage(int level, PixelTransferParams params) = 0;

		/**
		 * @brief Automatically generates all possible mipmaps for this texture.
		 *
		 * @throws std::logic_error The graphics context of this Texture2D was destroyed.
		 */
		virtual void generateMipmaps() = 0;

		[[nodiscard]]
		virtual int getWidth() const = 0;

		[[nodiscard]]
		virtual int getHeight() const = 0;

		[[nodiscard]]
		virtual int getChannels() const = 0;

		[[nodiscard]]
		virtual ImageFormat getFormat() const = 0;

	protected:
		ITexture2D() = default;

		ITexture2D(const ITexture2D &other) = default;

		ITexture2D(ITexture2D &&other) = default;

		ITexture2D &operator=(const ITexture2D &other) = default;

		ITexture2D &operator=(ITexture2D &&other) = default;
	};
}
