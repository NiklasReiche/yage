#include "Texture2D.h"

#include <cmath>

#include "Context.h"

namespace opengl
{
    Texture2D::~Texture2D()
    {
        lockContextPtr()->bindTexture(GL_TEXTURE_2D, 0);
    }

	Texture2D::Texture2D(Texture2D&&other) noexcept
		: Texture(std::move(other))
	{
		*this = std::move(other);
	}

	Texture2D& Texture2D::operator=(Texture2D&&other) noexcept
	{
		if (this != &other)
		{
			this->width = other.width;
			this->height = other.height;
			this->maxMipmapLevel = other.maxMipmapLevel;

			other.width = 0;
			other.height = 0;
			other.maxMipmapLevel = 0;

			Texture::operator=(std::move(other));
		}

		return *this;
	}

	int Texture2D::getMaxMipmapLevel() const
	{
		return static_cast<int>(std::floor(std::log2(std::max(width, height))));
	}

	void Texture2D::setImage(const std::span<const unsigned char>& data)
	{
		setImage(data, { getFormat(), gl::RowAlignment::B_1 });
	}

	void Texture2D::setImage(
	    const std::span<const unsigned char>& data,
	    gl::PixelTransferParams params)
	{
		setSubImage({ 0, 0, width, height }, data, params);
	}

	void Texture2D::setSubImage(
		utils::Area subArea,
	    const std::span<const unsigned char>& data)
	{
		setSubImage(subArea, data, { getFormat(), gl::RowAlignment::B_1 });
	}

	void Texture2D::setSubImage(
		utils::Area subArea,
	    const std::span<const unsigned char>& data,
		gl::PixelTransferParams params)
	{
		if (subArea.x < 0 || subArea.y < 0 || subArea.w <= 0 || subArea.h <= 0)
			throw std::invalid_argument("area dimensions cannot be negative");

		if (subArea.x + subArea.w > width || subArea.y + subArea.h > height)
			throw std::invalid_argument("the specified area is out of bounds");

		if (params.dataFormat == gl::ImageFormat::UNDEFINED)
			throw std::invalid_argument("image format cannot be undefined");

		const int channels = static_cast<int>(params.dataFormat);
		const int rowAlignment = static_cast<int>(params.rowAlignment);

		if (!gl::isDataSizeCorrect(subArea.w, subArea.h, channels, rowAlignment, data))
			throw std::invalid_argument("the input data container is incorrectly sized");


		auto ptr = lockContextPtr();
		ptr->setUnpackAlignment(rowAlignment);
		ptr->bindTexture(static_cast<GLenum>(target), texture);

		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			subArea.x,
			subArea.y,
			subArea.w,
			subArea.h,
			static_cast<GLenum>(convertToImageFormat(params.dataFormat)),
			pxType,
			data.data());
	}

	std::vector<unsigned char> Texture2D::getImage()
	{
		return getImage({ getFormat(), gl::RowAlignment::B_1 });
	}

	std::vector<unsigned char> Texture2D::getImage(const gl::PixelTransferParams params)
	{
		return getMipmapImage(0, params);
	}

	std::vector<unsigned char> Texture2D::getMipmapImage(int level)
	{
		return getMipmapImage(level, { getFormat(), gl::RowAlignment::B_1 });
	}

	std::vector<unsigned char> Texture2D::getMipmapImage(const int level, gl::PixelTransferParams params)
	{
		if (level > maxMipmapLevel)
			throw std::invalid_argument("there is no mipmap for the given level: <" + std::to_string(level) + ">");

		if (params.dataFormat == gl::ImageFormat::UNDEFINED)
			throw std::invalid_argument("image format cannot be undefined");

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
			pxType,
			data.data());

		return data;
	}

	void Texture2D::generateMipmaps()
	{
		auto ptr = lockContextPtr();
		ptr->bindTexture(static_cast<GLenum>(target), texture);

		glGenerateMipmap(static_cast<GLenum>(target));
		maxMipmapLevel = getMaxMipmapLevel();
	}

    void Texture2D::configTextureWrapper(gl::TextureWrapper xOption, gl::TextureWrapper yOption)
    {
        Texture::configTextureWrapper(xOption, yOption);
    }

    void Texture2D::configTextureFilter(gl::TextureFilter minOption, gl::TextureFilter magOption)
    {
        Texture::configTextureFilter(minOption, magOption);
    }

	int Texture2D::getWidth() const
	{
		return width;
	}

	int Texture2D::getHeight() const
	{
		return height;
	}

	int Texture2D::getChannels() const
	{
		return nChannels;
	}

	gl::ImageFormat Texture2D::getFormat() const
	{
		return convertToTextureFormat(format);
	}

    bool Texture2D::requires_mipmaps()
    {
        return Texture::requires_mipmaps();
    }
}
