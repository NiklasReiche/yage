#include "GLFW_Callback.h"
#include "GLFW_Handle.h"

namespace glfw
{
	void error_callback(int error, const char* description)
	{
		//throw GlfwException(GLFW_ERROR, description);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		GLFWHandle* handle = (GLFWHandle*)glfwGetWindowUserPointer(window);
		handle->onKeyEvent(key, scancode, action, mode);
	}
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		GLFWHandle* handle = (GLFWHandle*)glfwGetWindowUserPointer(window);
		handle->onMousePosEvent(xpos, ypos);
	}
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		GLFWHandle* handle = (GLFWHandle*)glfwGetWindowUserPointer(window);
		handle->onMouseButtonEvent(button, action, mods);
	}
	void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		GLFWHandle* handle = (GLFWHandle*)glfwGetWindowUserPointer(window);
		handle->onMouseWheelEvent(xoffset, yoffset);
	}
}