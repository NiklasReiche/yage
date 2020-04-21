#pragma once

#include <string>

#include <graphics/graphics.h>

#include "font.h"

namespace font
{
	class FontLoader
	{
	public:
		explicit FontLoader(gl::TextureCreator textureCreator) noexcept;

		[[nodiscard]]
		Font loadFont(const std::string& filename, int ptSize = 16, int dpi = 96) const;

	private:
		gl::TextureCreator textureCreator;

		static gml::Vec2<float> calcScale(int ptSize, int emSize, int dpiHorizontal = 0, int dpiVertical = 0);
	};
}