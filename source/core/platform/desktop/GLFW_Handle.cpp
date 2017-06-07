#include "GLFW_Handle.h"

namespace glfw
{
	GLFWHandle::GLFWHandle()
	{
		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) {
			//throw GlfwException(GLFW_ERROR, "GL::ERROR: Failed to initialize GLFW");
		}
	}
	GLFWHandle::~GLFWHandle()
	{
		glfwTerminate();
	}

	void GLFWHandle::createContext(int width, int height, std::string title, int versionMajor, int versionMinor)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (glfwWindow == nullptr) {
			glfwTerminate();
			//throw GlfwException(GLFW_ERROR, "GL::ERROR: Failed to create GLFW window");
		}

		glfwSetWindowUserPointer(glfwWindow, this);

		glfwMakeContextCurrent(glfwWindow);
		glfwSwapInterval(1);

		glfwSetKeyCallback(glfwWindow, key_callback);
		glfwSetCursorPosCallback(glfwWindow, mouse_callback);
		glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
		glfwSetScrollCallback(glfwWindow, mouse_wheel_callback);
	}

	void GLFWHandle::showWindow()
	{
		glfwShowWindow(glfwWindow);
	}
	void GLFWHandle::hideWindow()
	{
		glfwHideWindow(glfwWindow);
	}
	void GLFWHandle::makeCurrent()
	{
		glfwMakeContextCurrent(glfwWindow);
	}
	void GLFWHandle::pollEvents()
	{
		glfwPollEvents();
	}
	void GLFWHandle::swapBuffers()
	{
		glfwSwapBuffers(glfwWindow);
	}

	int GLFWHandle::shouldDestroy()
	{
		return glfwWindowShouldClose(glfwWindow);
	}


	void GLFWHandle::onKeyEvent(int key, int scancode, int action, int mode)
	{
		try {
			onKeyEventCallback(KeyCode(key), KeyAction(action));
		}
		catch (std::bad_function_call) {

		}
	}
	void GLFWHandle::onMousePosEvent(double xpos, double ypos)
	{
		try {
			onMousePosEventCallback(xpos, ypos);
		}
		catch (std::bad_function_call) {

		}
	}
	void GLFWHandle::onMouseButtonEvent(int button, int action, int mods)
	{
		try {
			onMouseButtonEventCallback(MouseKeyCode(button), KeyAction(action));
		}
		catch (std::bad_function_call) {

		}
	}
	void GLFWHandle::onMouseWheelEvent(double xoffset, double yoffset)
	{
		try {
			onMouseWheelEventCallback(xoffset, yoffset);
		}
		catch (std::bad_function_call) {

		}
	}
}