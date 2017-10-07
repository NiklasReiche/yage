#pragma once

#include <string>

#include <platform/platform.h>

#include "image.h"
#include "enum.h"

namespace img
{
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