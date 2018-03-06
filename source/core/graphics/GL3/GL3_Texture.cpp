#include "GL3_Texture.h"
#include "GL3_Context.h"
#include "GL3_Framebuffer.h"

namespace gl3
{
	GL3_Texture::GL3_Texture()
		: GL3_Object() {}
	GL3_Texture::GL3_Texture(const GL3_Texture& other)
		: GL3_Object(other)
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
	GL3_Texture::~GL3_Texture()
	{
		if (*refCount == 1) {
			glDeleteTextures(1, &texture);
		}
	}
	GL3_Texture& GL3_Texture::operator=(const GL3_Texture& other)
	{
		GL3_Object::operator=(other);
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

	void GL3_Texture::bufferSubData(int x_offset, int y_offset, int width, int height, unsigned char* data)
	{
		glContext->setUnpackAlignment(rowAlignment);
		glContext->bindTexture(*this);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, y_offset, width, height, GLenum(px_format), (GLenum)px_type, data);
	}
	void GL3_Texture::bufferSubData(int x_offset, int y_offset, int width, int height, std::vector<unsigned char> & data)
	{
		bufferSubData(x_offset, y_offset, width, height, &data[0]);
	}
	void GL3_Texture::bufferSubData(int x_offset, int y_offset, int width, int height, std::vector<unsigned char> && data)
	{
		bufferSubData(x_offset, y_offset, width, height, &data[0]);
	}
	void GL3_Texture::bufferSubData(int x_offset, int y_offset, GL3_Texture texture)
	{
		std::vector<unsigned char> data;
		glGetTexImage((GLenum)texture.target, (GLint)0, (GLenum)texture.px_format, (GLenum)texture.px_type, &data[0]);
		bufferSubData(x_offset, y_offset, texture.width, texture.height, data);
	}

	void GL3_Texture::bufferData(int width, int height, unsigned char* data)
	{
		glContext->setUnpackAlignment(rowAlignment);
		glContext->bindTexture(*this);
		glTexImage2D(GL_TEXTURE_2D, 0, GLenum(format), width, height, 0, (GLenum)px_format, (GLenum)px_type, data);

		this->width = width;
		this->height = height;
	}
	void GL3_Texture::bufferData(int width, int height, std::vector<unsigned char> & data)
	{
		bufferData(width, height, &data[0]);
	}

	void GL3_Texture::getTextureImage(std::vector<unsigned char> & data, int level) const
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

	void GL3_Texture::resize(int width, int height)
	{
		GLuint current_fbo = glContext->getGlState().renderTarget;
		Viewport current_viewport = glContext->getGlState().viewport;

		GL3_Framebuffer fbo = glContext->createFramebuffer(width, height, px_format);

		glContext->setActiveRenderTarget(fbo);
		glContext->setActiveViewport(Viewport(0, 0, width, height));
		glContext->draw(*this);

		operator=(fbo.getTexture());

		glContext->setActiveRenderTarget(current_fbo);
		glContext->setActiveViewport(current_viewport);
	}
	void GL3_Texture::rotate(int level)
	{
		GLuint current_fbo = glContext->getGlState().renderTarget;
		Viewport current_viewport = glContext->getGlState().viewport;

		GL3_Framebuffer fbo = glContext->createFramebuffer(width, height, px_format);

		glContext->setActiveRenderTarget(fbo);
		glContext->setActiveViewport(Viewport(0, 0, width, height));

		glContext->useUnitTransformShader(gml::Mat4f::axisAngle(gml::Vec3f(0.0f, 0.0f, 1.0f), level * 90));
		glContext->bindTexture(*this);
		glContext->drawUnitDrawable();

		operator=(fbo.getTexture());

		glContext->setActiveRenderTarget(current_fbo);
		glContext->setActiveViewport(current_viewport);
	}
}
