#include "GLFW_Handle.h"
#include "nativeFileDialog/nfd.h"
#include "../util.h"


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

		glfwSetCharModsCallback(glfwWindow, char_mods_callback);
		glfwSetKeyCallback(glfwWindow, key_callback);
		glfwSetCursorPosCallback(glfwWindow, mouse_callback);
		glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
		glfwSetScrollCallback(glfwWindow, mouse_wheel_callback);

		dpi = 96;
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

	void GLFWHandle::enableCharInput()
	{
		isCharInputEnabled = true;
	}
	void GLFWHandle::disableCharInput()
	{
		isCharInputEnabled = false;
	}
	void GLFWHandle::enableKeyInput()
	{
		isKeyInputEnabled = true;
	}
	void GLFWHandle::disableKeyInput()
	{
		isKeyInputEnabled = false;
	}

	double GLFWHandle::getTime()
	{
		return glfwGetTime();
	}
	double GLFWHandle::getTimeStep()
	{
		double now = glfwGetTime();
		double step = now - lastTimeStep;
		lastTimeStep = now;
		return step;
	}

	void GLFWHandle::onCharModsEvent(unsigned int codepoint, int mods)
	{
		if (isCharInputEnabled && onCharModsEventCallback) {
			onCharModsEventCallback(codepoint, mods);
		}
	}
	void GLFWHandle::onKeyEvent(int key, int scancode, int action, int mode)
	{
		if (isKeyInputEnabled && onKeyEventCallback) {
			onKeyEventCallback(key, action);
		}
	}
	void GLFWHandle::onMousePosEvent(float xpos, float ypos)
	{
		if (onMousePosEventCallback) {
			onMousePosEventCallback(xpos, ypos);
		}
	}
	void GLFWHandle::onMouseButtonEvent(int button, int action, int mods)
	{
		if (onMouseButtonEventCallback) {
			onMouseButtonEventCallback(button, action);
		}
	}
	void GLFWHandle::onMouseWheelEvent(float xoffset, float yoffset)
	{
		if (onMouseWheelEventCallback) {
			onMouseWheelEventCallback(xoffset, yoffset);
		}
	}

	
	void GLFWHandle::log(std::string msg)
	{
		std::cout << msg << std::endl;
	}
	Desktop_File GLFWHandle::open(std::string filename, AccessMode mode)
	{
		return Desktop_File(filename, mode);
	}
	std::string GLFWHandle::openFileDialog(std::string defaultPath)
	{
		nfdchar_t *outPath = nullptr;
		const nfdchar_t *default = nullptr;
		if (defaultPath.size() != 0) {
			default = util::replaceAll(defaultPath, "/", "\\").c_str();
		}
		nfdresult_t result = NFD_OpenDialog(nullptr, default, &outPath);

		switch (result)
		{
		case NFD_OKAY:
			return util::replaceAll(std::string(outPath), "\\", "/");
		case NFD_CANCEL:
			return "";
		case NFD_ERROR:
			throw sys::Exception(0);
		default:
			return "";
			break;
		}
	}
}