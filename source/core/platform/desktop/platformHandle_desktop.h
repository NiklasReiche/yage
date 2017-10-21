#pragma once

#include <string>
#include <iostream>
#include <functional>

#ifdef _WIN32
#define APIENTRY __stdcall // See GLAD documentation -- prevents windows.h macros
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../genericHandle.h"
#include "file_desktop.h"

namespace desktop
{
	class PlatformHandle : public sys::GenericPlatformHandle
	{
	private:
		GLFWwindow* glfwWindow;

		bool isCharInputEnabled = false;
		bool isKeyInputEnabled = true;
		float dpi;
		double lastTimeStep = 0.0;


		std::function<void(unsigned int, int)> onCharModsEventCallback;
		std::function<void(int, int)> onKeyEventCallback;
		std::function<void(float, float)> onMousePosEventCallback;
		std::function<void(int, int)> onMouseButtonEventCallback;
		std::function<void(float, float)> onMouseWheelEventCallback;

	public:
		PlatformHandle();
		~PlatformHandle();

		void createContext(int width, int height, std::string title = "OpenGL", int versionMajor = 3, int versionMinor = 3);

		void makeCurrent();
		void pollEvents();
		void swapBuffers();
		void showWindow();
		void hideWindow();

		void hideCursor();
		void showCursor();

		int shouldDestroy();

		void enableCharInput();
		void disableCharInput();
		void enableKeyInput();
		void disableKeyInput();

		float getDPI() { return dpi; }
		double getTime();
		double getTimeStep();

		void log(std::string msg);
		File open(std::string filename, AccessMode mode = AccessMode::READ);
		std::string openFileDialog(std::string defaultPath = "", std::vector<std::string> filterList = {}, std::string filterName = "");


		void onCharModsEvent(unsigned int codepoint, int mods);
		void onKeyEvent(int key, int scancode, int action, int mode);
		void onMousePosEvent(float xpos, float ypos);
		void onMouseButtonEvent(int button, int action, int mods);
		void onMouseWheelEvent(float xoffset, float yoffset);

		void setOnCharModsEvent(std::function<void(unsigned int, int)> callback) { onCharModsEventCallback = callback; }
		void setOnKeyEvent(std::function<void(int, int)> callback) { onKeyEventCallback = callback; }
		void setOnMousePosEvent(std::function<void(float, float)> callback) { onMousePosEventCallback = callback; }
		void setOnMouseButtonEvent(std::function<void(int, int)> callback) { onMouseButtonEventCallback = callback; }
		void setOnMouseWheelEvent(std::function<void(float, float)> callback) { onMouseWheelEventCallback = callback; }
	};
}
