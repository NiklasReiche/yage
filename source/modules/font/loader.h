#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "font.h"
#include "fileformat.h"

#include <IMG\img.h>


namespace font
{
	class FontLoader
	{
	private:
		gml::Vector2D<float> calcScale(int ptSize, int EM_size, int dpiHori = 0, int dpiVert = 0);
	public:
		Font loadFont(std::string filename, int ptsize = 16, int dpi = 96);
	};
}