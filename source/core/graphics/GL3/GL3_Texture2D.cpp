#include "GL3_Texture2D.h"

#include <cmath>

#include "GL3_Context.h"

namespace gl3
{
	GL3_Texture2D::GL3_Texture2D(GL3_Texture2D&& other) noexcept
		: GL3_Texture(std::move(other))
	{
		*this = std::move(other);
	}

	GL3_Texture2D& GL3_Texture2D::operator=(GL3_Texture2D&& other) noexcept
	{
		if (this != &other)
		{
			this->width = other.width;
			this->height = other.height;
			this->maxMipmapLevel = other.maxMipmapLevel;

			other.width = 0;
			other.height = 0;
			other.maxMipmapLevel = 0;

			GL3_Texture::operator=(std::move(other));
		}

		return *this;
	}

	int GL3_Texture2D::getMaxMipmapLevel() const
	{
		return static_cast<int>(std::floor(std::log2(std::max(width, height))));
	}

	void GL3_Texture2D::setImage(
		const std::vector<unsigned char>& data,
		const gl::PixelTransferParams params)
	{
		setSubImage(0, 0, width, height, data, params);
	}

	void GL3_Texture2D::setSubImage(
		const int xOffset,
		const int yOffset,
		const int width,
		const int height,
		const std::vector<unsigned char>& data,
		gl::PixelTransferParams params)
	{
		if (xOffset < 0 || yOffset < 0 || width <= 0 || height <= 0)
			throw std::invalid_argument("area dimensions cannot be negative");

		if (xOffset + width > this->width || yOffset + height > this->height)
			throw std::invalid_argument("the specified area is out of bounds");

		if (params.dataFormat == gl::ImageFormat::UNDEFINED)
			params.dataFormat = getFormat();

		const int channels = static_cast<int>(params.dataFormat);
		const int rowAlignment = static_cast<int>(params.rowAlignment);
		
		if (!gl::isDataSizeCorrect(width, height, channels, rowAlignment, data))
			throw std::invalid_argument("the input data container is incorrectly sized");


		auto ptr = lockContextPtr();
		ptr->setUnpackAlignment(rowAlignment);
		ptr->bindTexture(static_cast<GLenum>(target), texture);

		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			xOffset, 
			yOffset,
			width, 
			height,
			static_cast<GLenum>(convertToImageFormat(params.dataFormat)),
			GL_UNSIGNED_BYTE, 
			data.data());
	}

	std::vector<unsigned char> GL3_Texture2D::getImage(
		const gl::PixelTransferParams params)
	{
		return getMipmapImage(0, params);
	}

	std::vector<unsigned char> GL3_Texture2D::getMipmapImage(
		const int level,
		gl::PixelTransferParams params)
	{
		if (level > maxMipmapLevel)
			throw std::invalid_argument("there is no mipmap for the given level: <" + std::to_string(level) + ">");

		if (params.dataFormat == gl::ImageFormat::UNDEFINED)
			params.dataFormat = getFormat();

		const int rowAlignment = static_cast<int>(params.rowAlignment);
		
		auto ptr = lockContextPtr();
		ptr->setPackAlignment(rowAlignment);
		ptr->bindTexture(static_cast<GLenum>(target), texture);

		auto data = std::vector<unsigned char>(
			std::max(1, static_cast<int>(floor(width / pow(2, level)))) *
			std::max(1, static_cast<int>(floor(height / pow(2, level)))) *
			nChannels);

		const int levelWidth = std::max(1, static_cast<int>(std::floor(width / std::pow(2, level))));
		const int levelHeight = std::max(1, static_cast<int>(std::floor(height / std::pow(2, level))));
		data = std::vector<unsigned char>(gl::calculateImageDataSize(levelWidth, levelHeight, nChannels, rowAlignment));

		glGetTexImage(
			static_cast<GLenum>(target),
			static_cast<GLint>(level),
			static_cast<GLenum>(convertToImageFormat(params.dataFormat)),
			GL_UNSIGNED_BYTE,
			data.data());

		return data;
	}

	void GL3_Texture2D::generateMipmaps()
	{
		auto ptr = lockContextPtr();
		ptr->bindTexture(static_cast<GLenum>(target), texture);

		glGenerateMipmap(static_cast<GLenum>(target));
		maxMipmapLevel = getMaxMipmapLevel();
	}

	int GL3_Texture2D::getWidth() const
	{
		return width;
	}
	
	int GL3_Texture2D::getHeight() const
	{
		return height;
	}
	
	int GL3_Texture2D::getChannels() const
	{
		return nChannels;
	}

	gl::ImageFormat GL3_Texture2D::getFormat() const
	{
		return convertToTextureFormat(format);
	}
}
