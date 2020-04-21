#pragma once

#include <string>

#include "image.h"
#include "enum.h"

namespace img
{
	class ImageWriter
	{
	public:
		static void writeFile(Image & image, const std::string & path);

	private:
		static void writeBmp(
			const std::string & filename, 
			Image & image, 
			FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);
	};
}