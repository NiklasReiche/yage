#pragma once

#include <array>
#include <map>

#include <math\MVector.h>

#include "Keyboard.h"

namespace glfw
{
	struct GLFW_Input
	{
		std::map<KeyCode, KeyState> keys;
		gml::Vec2<double> mouse_pos;
		gml::Vec2<double> mouse_wheel_offset;
	};
}