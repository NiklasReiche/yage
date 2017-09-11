#pragma once

#include <string>
#include <platform/platform.h>
#include "img.h"
#include "png.h"

namespace img
{
	class ImageReader
	{
	private:
		sys::PlatformHandle* platform;

		void readBMP(std::string filename, Image & image, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);
		void readPNG(std::string filename, Image & image);

	public:
		ImageReader(sys::PlatformHandle* platform)
			: platform(platform){}
		
		Image readFile(std::string path);
	};

	class ImageWriter
	{
	private:
		sys::PlatformHandle* platform;

		void writeBMP(std::string filename, Image & image, FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);

	public:
		ImageWriter(sys::PlatformHandle* platform)
			: platform(platform) {}

		void writeFile(Image& image, std::string path);
	};
}