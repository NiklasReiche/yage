#pragma once

#include <string>

#include <platform/platform.h>

#include "image.h"
#include "enum.h"

namespace img
{
	class ImageReader
	{
	private:
		sys::PlatformHandle* platform;

		void readBMP(std::string filename, Image & image, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);
		void readPNG(std::string filename, Image & image, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);

	public:
		ImageReader(sys::PlatformHandle* platform)
			: platform(platform){}
		
		Image readFile(std::string path, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);
		Image readTexture(gl::Texture& texture, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);
	};
}