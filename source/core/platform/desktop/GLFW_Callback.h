#pragma once

#include "GLFW_Handle.h"

namespace glfw
{
	void error_callback(int error, const char* description);

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
}