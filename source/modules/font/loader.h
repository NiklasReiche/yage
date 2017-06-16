#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <platform/Platform.h>
#include <graphics/Graphics.h>

#include "fileformat.h"
#include "font.h"

namespace font
{
	class FontLoader
	{
	private:
		platform::PlatformHandle* platform;
		gl::GraphicsContext* glContext;

		gml::Vector2D<float> calcScale(int ptSize, int EM_size, int dpiHori = 0, int dpiVert = 0);

	public:
		FontLoader(platform::PlatformHandle* platform, gl::GraphicsContext * glContext);

		Font loadFont(std::string filename, int ptsize = 16, int dpi = 96);
	};
}