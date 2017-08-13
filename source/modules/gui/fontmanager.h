#pragma once

#include <string>
#include <map>

#include "core.h"

namespace gui
{
	class FontManager
	{
	private:
		sys::PlatformHandle* platform;
		font::FontLoader fontLoader;
		std::map<std::string, font::Font> fonts;

	public:
		FontManager(sys::PlatformHandle* platform, gl::GraphicsContext * glContext);

		void addFont(std::string filename);
		void removeFont(std::string name);
		font::Font& getFont(std::string name);
	};
}