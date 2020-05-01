#include "GL3_TextureCreator.h"

#include "GL3_Context.h"
#include "GL3_Texture2D.h"
#include "GL3_Cubemap.h"

namespace gl3
{
	std::unique_ptr<gl::ITexture2D>
	GL3_TextureCreator::createTexture2D(
		int width,
		int height,
		gl::ImageFormat textureFormat,
		const std::vector<unsigned char> &data)
	{
		return createTexture2D(width, height, textureFormat, data,
		                       { textureFormat, gl::RowAlignment::B_1 });
	}

	std::unique_ptr<gl::ITexture2D> GL3_TextureCreator::createTexture2D(
		const int width,
		const int height,
		const gl::ImageFormat textureFormat,
		const std::vector<unsigned char> &data,
		const gl::PixelTransferParams params)
	{
		if (width <= 0 || height <= 0)
			throw std::invalid_argument("texture dimensions must be strictly positive");

		if (textureFormat == gl::ImageFormat::UNDEFINED || params.dataFormat == gl::ImageFormat::UNDEFINED)
			throw std::invalid_argument("image format cannot be undefined");

		const int nChannels = static_cast<int>(params.dataFormat);
		const int rowAlignment = static_cast<int>(params.rowAlignment);

		if (!data.empty() && !gl::isDataSizeCorrect(width, height, nChannels, rowAlignment, data))
			throw std::invalid_argument("the input data container is incorrectly sized");

		auto ptr = lockContextPtr();
		auto texture = std::unique_ptr<GL3_Texture2D>(new GL3_Texture2D(ptr));

		// generate gpu object
		glGenTextures(1, &texture->id);

		// bind object
		ptr->bindTexture(GL_TEXTURE_2D, texture->id);

		// set pixel transfer parameters
		InternalFormat internalFormat = convertToInternalFormat(textureFormat);
		ImageFormat imageFormat = convertToImageFormat(params.dataFormat);
		ptr->setUnpackAlignment(rowAlignment);

		// upload data to gpu
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			static_cast<GLenum>(internalFormat),
			width,
			height,
			0,
			static_cast<GLenum>(imageFormat),
			texture->pxType,
			data.empty() ? nullptr : data.data());

		// set default filter & wrapper options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// construct cpu object
		texture->texture = texture->id;

		texture->width = width;
		texture->height = height;
		texture->nChannels = static_cast<int>(textureFormat);

		texture->target = TextureType::TEXTURE_2D;
		texture->format = internalFormat;

		return texture;
	}

	std::unique_ptr<gl::ICubemap> GL3_TextureCreator::createCubemap(
		int width,
		int height,
		gl::ImageFormat textureFormat,
		const std::array<std::vector<unsigned char>, 6> &data)
	{
		return createCubemap(width, height, textureFormat, data, { textureFormat, gl::RowAlignment::B_1 });
	}

	std::unique_ptr<gl::ICubemap> GL3_TextureCreator::createCubemap(
		int width,
		int height,
		gl::ImageFormat textureFormat,
		const std::array<std::vector<unsigned char>, 6> &data,
		gl::PixelTransferParams params)
	{
		if (width <= 0 || height <= 0)
			throw std::invalid_argument("texture dimensions must be strictly positive");

		if (textureFormat == gl::ImageFormat::UNDEFINED || params.dataFormat == gl::ImageFormat::UNDEFINED)
			throw std::invalid_argument("image format cannot be undefined");

		const int nChannels = static_cast<int>(params.dataFormat);
		const int rowAlignment = static_cast<int>(params.rowAlignment);

		for (const auto &image : data)
		{
			if (!image.empty() && !gl::isDataSizeCorrect(width, height, nChannels, rowAlignment, image))
				throw std::invalid_argument("the input data container is incorrectly sized");
		}


		auto ptr = lockContextPtr();
		auto texture = std::unique_ptr<GL3_Cubemap>(new GL3_Cubemap(ptr));

		// generate gpu object
		glGenTextures(1, &texture->id);

		// bind object
		ptr->bindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

		// set pixel transfer parameters
		InternalFormat internalFormat = convertToInternalFormat(textureFormat);
		ImageFormat imageFormat = convertToImageFormat(params.dataFormat);
		ptr->setUnpackAlignment(rowAlignment);

		// upload data to gpu
		for (GLuint i = 0; i < 6; i++)
		{
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				static_cast<GLenum>(internalFormat),
				width,
				height,
				0,
				static_cast<GLenum>(imageFormat),
				texture->pxType,
				data[i].empty() ? nullptr : data[i].data());
		}

		// set default filter & wrapper options
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// construct cpu object
		texture->texture = texture->id;

		texture->width = width;
		texture->height = height;
		texture->nChannels = static_cast<int>(textureFormat);

		texture->target = TextureType::TEXTURE_CUBEMAP;
		texture->format = internalFormat;

		return texture;
	}
}
