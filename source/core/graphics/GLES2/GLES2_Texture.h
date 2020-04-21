#pragma once

#include <vector>

#include "gles2.h"
#include "GLES2_Object.h"
#include "GLES2_Enum.h"

namespace gles2
{
	class GLES2_Texture : public GLES2_Object
	{
	private:
		friend class GLES2_Context;

		GLuint texture = 0;
		int width = 0;
		int height = 0;
		TextureType target;
		InternalFormat format;
		ImageFormat px_format;
		PixelType px_type;
		int rowAlignment = 4;
		int nChannels = 0;

	public:
		GLES2_Texture();
		GLES2_Texture(const GLES2_Texture& other);
		virtual ~GLES2_Texture();
		GLES2_Texture& operator=(const GLES2_Texture& other);

		void bufferSubData(int x_offset, int y_offset, int width, int height, std::vector<unsigned char> & data);
		void getTextureImage(std::vector<unsigned char> & data, int level = 0);

		void configTextureWrapper(TextureWrapper x_option, TextureWrapper y_option);
		void configTextureFilter(TextureFilter min_option, TextureFilter mag_option);
		void configTextureFilter(MipmapOption min_option, TextureFilter mag_option);

		int getWidth() const { return width; }
		int getHeight() const { return height; }
	};
}
