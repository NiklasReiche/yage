#include "GL3_TextureCreator.h"

#include "GL3_Context.h"
#include "GL3_Texture2D.h"
#include "utils/NotImplementedException.h"

namespace gl3
{
	std::unique_ptr<gl::ITexture2D> GL3_TextureCreator::createTexture2D(
		const std::vector<unsigned char> & data,
		const int width,
		const int height,
		const gl::ImageFormat format,
		const int rowAlignment)
	{
		auto ptr = lockContextPtr();
		auto texture = std::unique_ptr<GL3_Texture2D>(new GL3_Texture2D(ptr));
		
		// generate gpu object
		glGenTextures(1, &texture->id);

		// bind object
		ptr->bindTexture(GL_TEXTURE_2D, texture->id);

		// select pixel pack format
		InternalFormat internalFormat = GL3_Context::convertToInternalFormat(format);
		ImageFormat imageFormat = GL3_Context::convertToImageFormat(format);

		// set unpack alignment
		ptr->setUnpackAlignment(rowAlignment);

		// upload data to gpu
		glTexImage2D(
			GL_TEXTURE_2D, 0, static_cast<GLenum>(internalFormat),
			width, height, 0,
			static_cast<GLenum>(imageFormat), static_cast<GLenum>(PixelType::U_BYTE),
			data.empty() ? nullptr : &(data[0]));

		// set default filter & wrapper options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// construct cpu object
		texture->texture = texture->id;

		texture->height = height;
		texture->width = width;

		texture->target = TextureType::TEXTURE_2D;
		texture->format = internalFormat;

		texture->pxType = PixelType::U_BYTE;
		texture->pxFormat = imageFormat;
		texture->rowAlignment = rowAlignment;
		texture->nChannels = GL3_Context::convertToChannelSize(imageFormat);

		return texture;
	}

	std::unique_ptr<gl::ITexture3D> GL3_TextureCreator::createTexture3D(
		const std::vector<unsigned char> & data,
		int width,
		int height,
		int depth,
		gl::ImageFormat format,
		int rowAlignment)
	{
		throw NotImplementedException();
	}

	std::unique_ptr<gl::ICubemap> GL3_TextureCreator::createCubemap(
		const std::array<std::vector<unsigned char>, 6> & data,
		int width,
		int height,
		gl::ImageFormat format,
		int rowAlignment)
	{
#if 0
		// Generate & Bind
		std::shared_ptr<GL3_Texture> texture = std::make_shared<GL3_Texture>();
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);

		// Select pixel pack format
		InternalFormat internalFormat = convertToInternalFormat(format);

		// Set unpack alignment
		setUnpackAlignment(rowAlignment);

		// Attach Textures
		for (GLuint i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
				width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &(data[i][0]));
		}

		// Set Image Wrapper
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// Set Image Filter
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Unbind
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		// Build Texture
		texture->texture = texture->id;
		texture->glContext = this;

		texture->height = height;
		texture->width = width;

		texture->target = TextureType::TEXTURE_CUBEMAP;
		texture->format = internalFormat;

		texture->px_type = PixelType::U_BYTE;
		texture->px_format = convertToImageFormat(format);
		texture->rowAlignment = rowAlignment;
		texture->nChannels = convertToChannelSize(texture->px_format);

		return texture;
#endif
		throw NotImplementedException();
	}
}