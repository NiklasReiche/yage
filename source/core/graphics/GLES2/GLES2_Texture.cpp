#include "GLES2_Texture.h"
#include "GLES2_Context.h"

namespace gles2
{
	GLES2_Texture::GLES2_Texture()
		: GLES2_Object() {}
	GLES2_Texture::GLES2_Texture(const GLES2_Texture& other)
		: GLES2_Object(other)
	{
		this->texture = other.texture;
		this->width = other.width;
		this->height = other.height;
		this->target = other.target;
		this->format = other.format;
		this->px_format = other.px_format;
		this->px_type = other.px_type;
		this->rowAlignment = other.rowAlignment;
		this->nChannels = other.nChannels;
	}
	GLES2_Texture::~GLES2_Texture()
	{
		if (*refCount == 1) {
			glDeleteTextures(1, &texture);
		}
	}
	GLES2_Texture& GLES2_Texture::operator=(const GLES2_Texture& other)
	{
		GLES2_Object::operator=(other);
		if (shouldDelete) {
			glDeleteTextures(1, &texture);
			shouldDelete = false;
		}

		this->texture = other.texture;
		this->width = other.width;
		this->height = other.height;
		this->target = other.target;
		this->format = other.format;
		this->px_format = other.px_format;
		this->px_type = other.px_type;
		this->rowAlignment = other.rowAlignment;
		this->nChannels = other.nChannels;

		return *this;
	}

	void GLES2_Texture::bufferSubData(int x_offset, int y_offset, int width, int height, std::vector<unsigned char> & data)
	{
		glContext->setUnpackAlignment(rowAlignment);
		glContext->bindTexture(*this);

		glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, width, height, GLenum(px_format), (GLenum)px_type, &data[0]);
	}

	void GLES2_Texture::getTextureImage(std::vector<unsigned char> & data, int level)
	{
		glContext->setPackAlignment(rowAlignment);
		glContext->bindTexture(*this);

		data.resize((width / (int)pow(2, level)) * (height / (int)pow(2, level)) * nChannels);

		//glGetTexImage((GLenum)target, (GLint)level, (GLenum)px_format, (GLenum)px_type, &data[0]);
	}

	void GLES2_Texture::configTextureWrapper(TextureWrapper x_option, TextureWrapper y_option)
	{
		glContext->bindTexture(*this, 0);

		glTexParameteri((GLenum)target, GL_TEXTURE_WRAP_S, (GLenum)x_option);
		glTexParameteri((GLenum)target, GL_TEXTURE_WRAP_T, (GLenum)y_option);
	}
	void GLES2_Texture::configTextureFilter(TextureFilter min_option, TextureFilter mag_option)
	{
		glContext->bindTexture(*this, 0);

		glTexParameteri((GLenum)target, GL_TEXTURE_MIN_FILTER, (GLenum)min_option);
		glTexParameteri((GLenum)target, GL_TEXTURE_MAG_FILTER, (GLenum)mag_option);
	}
	void GLES2_Texture::configTextureFilter(MipmapOption min_option, TextureFilter mag_option)
	{
		glContext->bindTexture(*this, 0);

		glGenerateMipmap((GLenum)target);
		glTexParameteri((GLenum)target, GL_TEXTURE_MIN_FILTER, (GLenum)min_option);
		glTexParameteri((GLenum)target, GL_TEXTURE_MAG_FILTER, (GLenum)mag_option);
	}
}