#pragma once

#include <vector>

#include "gl3.h"
#include "GL3_Object.h"
#include "GL3_Enum.h"

namespace gl3
{
	class GL3_Texture : public GL3_Object
	{
	private:
		friend class GL3_Context;

		GLuint texture;
		int width, height;
		TextureType target;
		InternalFormat format;
		ImageFormat px_format;
		PixelType px_type;
		int rowAlignment = 4;
		int nChannels;

	public:
		GL3_Texture();
		GL3_Texture(const GL3_Texture& other);
		virtual ~GL3_Texture();
		GL3_Texture& operator=(const GL3_Texture& other);

		void bufferSubData(int x_offset, int y_offset, int width, int height, std::vector<unsigned char> & data);
		void getTextureImage(std::vector<unsigned char> & data, int level = 0);

		void configTextureWrapper(TextureWrapper x_option, TextureWrapper y_option);
		void configTextureFilter(TextureFilter min_option, TextureFilter mag_option);
		void configTextureFilter(MipmapOption min_option, TextureFilter mag_option);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
	};
}