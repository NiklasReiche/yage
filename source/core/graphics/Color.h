#pragma once

#include "math\MVector.h"

namespace gl
{
	namespace Color
	{
		enum Color
		{
			BLACK = 0x000000FFu,
			WHITE = 0xFFFFFFFFu,

			RED = 0xFF0000FFu,
			GREEN = 0x00FF00FFu,
			BLUE = 0x0000FFFFu,

			CYAN = 0x00FFFFFFu,
			DARK_BLUE = 0x0000A0FFu,
			LIGHT_BLUE = 0xADD8E6FFu,

			PURPLE = 0x800080FFu,
			YELLOW = 0xFFFF00FFu,
			LIME = 0x00FF00FFu,
			MAGENTA = 0xFF00FFFFu,
			SILVER = 0xC0C0C0FFu,
			GREY = 0x808080FFu,
			ORANGE = 0xFFA500FFu,
			BROWN = 0xA52A2AFFu,
			MAROON = 0x800000FFu,
			OLIVE = 0x808000FFu
		};
	}

	inline void setRed(unsigned int & color, unsigned int red)
	{
		color |= red << 24;
	}
	inline void setGreen(unsigned int & color, unsigned int green)
	{
		color |= green << 16;
	}
	inline void setBlue(unsigned int & color, unsigned int blue)
	{
		color |= blue << 8;
	}
	inline void setAlpha(unsigned int & color, unsigned int alpha)
	{
		color |= alpha;
	}

	inline gml::Vec3<float> toVec3(unsigned int color)
	{
		return gml::Vec3<float>(((color >> 24) & 0xff) / 255.0f, ((color >> 16) & 0xff) / 255.0f, ((color >> 8) & 0xff) / 255.0f);
	}
	inline gml::Vec4<float> toVec4(unsigned int color)
	{
		return gml::Vec4<float>(((color >> 24) & 0xff) / 255.0f, ((color >> 16) & 0xff) / 255.0f, ((color >> 8) & 0xff) / 255.0f, ((color) & 0xff) / 255.0f);
	}
}