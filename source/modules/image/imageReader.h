#pragma once

#include <string>

#include <graphics/Texture2D.h>

#include "image.h"
#include "enum.h"

namespace img
{
	class ImageReader
	{
	public:
		static Image readFile(
			const std::string & path, 
			FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);

		static Image readTexture(
			const gl::Texture2D & texture, 
			FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);

	private:
		static void readBmp(
			const std::string & filename, Image & image, 
			FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);

		static void readPng(
			const std::string & filename, Image & image, 
			FORCE_CHANNELS channel = FORCE_CHANNELS::AUTO);
	};
}