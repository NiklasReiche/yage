#include "GL3_Texture2D.h"
#include "GL3_Context.h"

namespace gl3
{
	GL3_Texture2D::GL3_Texture2D(GL3_Texture2D&& other) noexcept
		: GL3_Texture(std::move(other))
	{
		this->width = other.width;
		this->height = other.height;

		other.width = 0;
		other.height = 0;
	}

	GL3_Texture2D& GL3_Texture2D::operator=(GL3_Texture2D&& other) noexcept
	{
		if (this != &other)
		{
			this->width = other.width;
			this->height = other.height;

			other.width = 0;
			other.height = 0;
		}

		return *this;
	}
	
	void GL3_Texture2D::setSubData(
		const int xOffset, const int yOffset, 
		const int width, const int height, unsigned char * data)
	{
		auto ptr = lockContextPtr();
		ptr->setUnpackAlignment(rowAlignment);
		ptr->bindTexture(static_cast<GLenum>(target), texture);
		
		glTexSubImage2D(
			GL_TEXTURE_2D, 0, 
			xOffset, yOffset, 
			width, height, 
			static_cast<GLenum>(pxFormat), static_cast<GLenum>(pxType), 
			data);
	}
	
	void GL3_Texture2D::setSubData(
		const int xOffset, const int yOffset,
		const int width, const int height, std::vector<unsigned char>& data)
	{
		setSubData(xOffset, yOffset, width, height, &data[0]);
	}

	void GL3_Texture2D::setData(
		const int width, const int height, unsigned char * data)
	{
		auto ptr = lockContextPtr();
		ptr->setUnpackAlignment(rowAlignment);
		ptr->bindTexture(static_cast<GLenum>(target), texture);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GLenum(format), 
			width, height, 0, 
			static_cast<GLenum>(pxFormat), 
			static_cast<GLenum>(pxType), data);

		this->width = width;
		this->height = height;
	}
	void GL3_Texture2D::setData(
		const int width, const int height, std::vector<unsigned char>& data)
	{
		setData(width, height, &data[0]);
	}

	void GL3_Texture2D::getData(std::vector<unsigned char>& data, const int level)
	{
		auto ptr = lockContextPtr();
		ptr->setPackAlignment(rowAlignment);
		ptr->bindTexture(static_cast<GLenum>(target), texture);

		data.resize(
			width / static_cast<int>(pow(2, level)) * 
			(height / static_cast<int>(pow(2, level))) * 
			nChannels);

		glGetTexImage(
			static_cast<GLenum>(target), 
			static_cast<GLint>(level), 
			static_cast<GLenum>(pxFormat), 
			static_cast<GLenum>(pxType), 
			&data[0]);
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

	bool GL3_Texture2D::isEmpty() const
	{
		
	}
}
