#include "GL3_Texture.h"
#include "GL3_Context.h"

namespace gl3
{
	void GL3_Texture::bufferSubData(int x_offset, int y_offset, int width, int height, std::vector<unsigned char> & data)
	{
		glContext->setUnpackAlignment(rowAlignment);
		glContext->bindTexture(*this);

		glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, width, height, GLenum(px_format), (GLenum)px_type, &data[0]);
	}

	void GL3_Texture::getTextureImage(std::vector<unsigned char> & data, int level)
	{
		glContext->setPackAlignment(rowAlignment);
		glContext->bindTexture(*this);

		data.resize((width / (int)pow(2, level)) * (height / (int)pow(2, level)) * nChannels);

		glGetTexImage((GLenum)target, (GLint)level, (GLenum)px_format, (GLenum)px_type, &data[0]);
	}

	void GL3_Texture::configTextureWrapper(TextureWrapper x_option, TextureWrapper y_option)
	{
		glContext->bindTexture(*this, 0);

		glTexParameteri((GLenum)target, GL_TEXTURE_WRAP_S, (GLenum)x_option);
		glTexParameteri((GLenum)target, GL_TEXTURE_WRAP_T, (GLenum)y_option);
	}
	void GL3_Texture::configTextureFilter(TextureFilter min_option, TextureFilter mag_option)
	{
		glContext->bindTexture(*this, 0);

		glTexParameteri((GLenum)target, GL_TEXTURE_MIN_FILTER, (GLenum)min_option);
		glTexParameteri((GLenum)target, GL_TEXTURE_MAG_FILTER, (GLenum)mag_option);
	}
	void GL3_Texture::configTextureFilter(MipmapOption min_option, TextureFilter mag_option)
	{
		glContext->bindTexture(*this, 0);

		glGenerateMipmap((GLenum)target);
		glTexParameteri((GLenum)target, GL_TEXTURE_MIN_FILTER, (GLenum)min_option);
		glTexParameteri((GLenum)target, GL_TEXTURE_MAG_FILTER, (GLenum)mag_option);
	}
}