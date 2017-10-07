#include "glfwCallback.h"

namespace desktop
{
	void error_callback(int error, const char* description)
	{
		//throw GlfwException(GLFW_ERROR, description);
	}

	void char_mods_callback(GLFWwindow* window, unsigned int codepoint, int mods)
	{
		PlatformHandle* handle = (PlatformHandle*)glfwGetWindowUserPointer(window);
		handle->onCharModsEvent(codepoint, mods);
	}
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		PlatformHandle* handle = (PlatformHandle*)glfwGetWindowUserPointer(window);
		handle->onKeyEvent(key, scancode, action, mode);
	}
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		PlatformHandle* handle = (PlatformHandle*)glfwGetWindowUserPointer(window);
		handle->onMousePosEvent((float)xpos, (float)ypos);
	}
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		PlatformHandle* handle = (PlatformHandle*)glfwGetWindowUserPointer(window);
		handle->onMouseButtonEvent(button, action, mods);
	}
	void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		PlatformHandle* handle = (PlatformHandle*)glfwGetWindowUserPointer(window);
		handle->onMouseWheelEvent((float)xoffset, (float)yoffset);
	}
}