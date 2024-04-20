#pragma once

#include <string>

#include <core/gl/graphics.h>

#include "font.h"
#include "core/platform/IBinaryFile.h"

namespace font
{
	class FontLoader
	{
	public:
		explicit FontLoader(const std::shared_ptr<gl::ITextureCreator>& textureCreator) noexcept;

		[[nodiscard]]
		std::unique_ptr<Font> loadFont(platform::IBinaryFile& file, int ptSize = 16, int dpi = 96) const;

	private:
        std::shared_ptr<gl::ITextureCreator> textureCreator;

		static gml::Vec2<float> calcScale(int ptSize, int emSize, int dpiHorizontal = 0, int dpiVertical = 0);
	};
}