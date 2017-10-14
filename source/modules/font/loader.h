#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <platform/platform.h>
#include <graphics/graphics.h>

#include "fileformat.h"
#include "font.h"

namespace font
{
	class FontLoader
	{
	private:
		sys::PlatformHandle* platform;
		gl::GraphicsContext* glContext;

		gml::Vec2<float> calcScale(int ptSize, int EM_size, int dpiHori = 0, int dpiVert = 0);

	public:
		FontLoader(sys::PlatformHandle* platform, gl::GraphicsContext * glContext);

		Font loadFont(std::string filename, int ptsize = 16, int dpi = 96);
	};
}