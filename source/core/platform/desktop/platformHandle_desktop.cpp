#include "platformHandle_desktop.h"
#include "glfwCallback.h"
#include "../util.h"
#include <nfd/nfd.h>

namespace desktop
{
	PlatformHandle::PlatformHandle()
	{
		glfwSetErrorCallback(error_callback);

		if (!glfwInit()) {
			//throw GlfwException(GLFW_ERROR, "GL::ERROR: Failed to initialize GLFW");
		}
	}
	PlatformHandle::~PlatformHandle()
	{
		glfwTerminate();
	}

	void PlatformHandle::createContext(int width, int height, std::string title, int versionMajor, int versionMinor)
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

	void PlatformHandle::showWindow()
	{
		glfwShowWindow(glfwWindow);
	}
	void PlatformHandle::hideWindow()
	{
		glfwHideWindow(glfwWindow);
	}
	void PlatformHandle::makeCurrent()
	{
		glfwMakeContextCurrent(glfwWindow);
	}
	void PlatformHandle::pollEvents()
	{
		glfwPollEvents();
	}
	void PlatformHandle::swapBuffers()
	{
		glfwSwapBuffers(glfwWindow);
	}

	void PlatformHandle::hideCursor()
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	void PlatformHandle::showCursor()
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	int PlatformHandle::shouldDestroy()
	{
		return glfwWindowShouldClose(glfwWindow);
	}

	void PlatformHandle::enableCharInput()
	{
		isCharInputEnabled = true;
	}
	void PlatformHandle::disableCharInput()
	{
		isCharInputEnabled = false;
	}
	void PlatformHandle::enableKeyInput()
	{
		isKeyInputEnabled = true;
	}
	void PlatformHandle::disableKeyInput()
	{
		isKeyInputEnabled = false;
	}

	double PlatformHandle::getTime()
	{
		return glfwGetTime();
	}
	double PlatformHandle::getTimeStep()
	{
		double now = glfwGetTime();
		double step = now - lastTimeStep;
		lastTimeStep = now;
		return step;
	}

	void PlatformHandle::onCharModsEvent(unsigned int codepoint, int mods)
	{
		if (isCharInputEnabled && onCharModsEventCallback) {
			onCharModsEventCallback(codepoint, mods);
		}
	}
	void PlatformHandle::onKeyEvent(int key, int scancode, int action, int mode)
	{
		if (isKeyInputEnabled && onKeyEventCallback) {
			onKeyEventCallback(key, action);
		}
	}
	void PlatformHandle::onMousePosEvent(float xpos, float ypos)
	{
		if (onMousePosEventCallback) {
			onMousePosEventCallback(xpos, ypos);
		}
	}
	void PlatformHandle::onMouseButtonEvent(int button, int action, int mods)
	{
		if (onMouseButtonEventCallback) {
			onMouseButtonEventCallback(button, action);
		}
	}
	void PlatformHandle::onMouseWheelEvent(float xoffset, float yoffset)
	{
		if (onMouseWheelEventCallback) {
			onMouseWheelEventCallback(xoffset, yoffset);
		}
	}

	
	void PlatformHandle::log(std::string msg)
	{
		std::cout << msg << std::endl;
	}
	File PlatformHandle::open(std::string filename, AccessMode mode)
	{
		return File(filename, mode);
	}
	std::string PlatformHandle::openFileDialog(std::string defaultPath)
	{
		nfdchar_t *outPath = nullptr;
		const nfdchar_t *defaultPathC = nullptr;
		if (defaultPath.size() != 0) {
			defaultPathC = util::replaceAll(defaultPath, "/", "\\").c_str();
		}
		nfdresult_t result = NFD_OpenDialog(nullptr, defaultPathC, &outPath);
		std::string outPathS = std::string(outPath);

		switch (result)
		{
		case NFD_OKAY:
			return util::replaceAll(outPathS, "\\", "/");
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