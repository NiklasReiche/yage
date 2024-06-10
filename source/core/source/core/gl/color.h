#pragma once

#include <cstdint>

#include <math/vector.h>

namespace yage::gl
{
    typedef uint32_t Color_t;

	namespace Color
	{
		enum Color : Color_t
		{
			TRANSPARENT = 0x00000000u,
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
            LIGHT_GREY = 0xD0D0D0FFu,
			ORANGE = 0xFFA500FFu,
			BROWN = 0xA52A2AFFu,
			MAROON = 0x800000FFu,
			OLIVE = 0x808000FFu
		};
	}

	inline void setRed(Color_t & color, uint32_t red)
	{
		unsigned int g = (color >> 16) & 0xff;
		unsigned int b = (color >> 8) & 0xff;
		unsigned int a = (color) & 0xff;
		color = (a) | (b << 8) | (g << 16) | (red << 24);
	}

	inline void setGreen(Color_t & color, uint32_t green)
	{
		unsigned int r = (color >> 24) & 0xff;
		unsigned int b = (color >> 8) & 0xff;
		unsigned int a = (color) & 0xff;
		color = (a) | (b << 8) | (green << 16) | (r << 24);
	}

	inline void setBlue(Color_t & color, uint32_t blue)
	{
		unsigned int r = (color >> 24) & 0xff;
		unsigned int g = (color >> 16) & 0xff;
		unsigned int a = (color) & 0xff;
		color = (a) | (blue << 8) | (g << 16) | (r << 24);
	}

	inline void setAlpha(Color_t & color, uint32_t alpha)
	{
		unsigned int r = (color >> 24) & 0xff;
		unsigned int g = (color >> 16) & 0xff;
		unsigned int b = (color >> 8) & 0xff;
		color = (alpha) | (b << 8) | (g << 16) | (r << 24);
	}

	inline math::Vec3f toVec3(Color_t color)
	{
		return {
            static_cast<float>((color >> 24) & 0xff) / 255.0f,
            static_cast<float>((color >> 16) & 0xff) / 255.0f,
            static_cast<float>((color >> 8) & 0xff) / 255.0f
        };
	}

	inline math::Vec4f toVec4(Color_t color)
	{
		return {
            static_cast<float>((color >> 24) & 0xff) / 255.0f,
            static_cast<float>((color >> 16) & 0xff) / 255.0f,
            static_cast<float>((color >> 8) & 0xff) / 255.0f,
            static_cast<float>((color) & 0xff) / 255.0f
        };
	}
}