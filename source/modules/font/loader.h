#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "font.h"
#include "fileformat.h"

#include <image\img.h>
#include <graphics\Graphics.h>

namespace font
{
	class FontLoader
	{
	private:
		gl::GraphicsContext* glContext;

		gml::Vector2D<float> calcScale(int ptSize, int EM_size, int dpiHori = 0, int dpiVert = 0);

	public:
		FontLoader(gl::GraphicsContext * glContext);

		Font loadFont(std::string filename, int ptsize = 16, int dpi = 96);
	};
}