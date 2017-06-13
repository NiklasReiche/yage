#pragma once

#include <functional>

#include <GLFW\glfw3.h>
#include "GLFW_Callback.h"

#include <platform\GenericHandle.h>

namespace glfw
{
	class GLFWHandle : public platform::GenericPlatformHandle
	{
	private:
		GLFWwindow* glfwWindow;

		std::function<void(int, int)> onKeyEventCallback;
		std::function<void(float, float)> onMousePosEventCallback;
		std::function<void(int, int)> onMouseButtonEventCallback;
		std::function<void(float, float)> onMouseWheelEventCallback;

	public:
		GLFWHandle();
		~GLFWHandle();

		void createContext(int width, int height, std::string title = "OpenGL", int versionMajor = 3, int versionMinor = 3);

		void makeCurrent();
		void pollEvents();
		void swapBuffers();
		void showWindow();
		void hideWindow();

		int shouldDestroy();

		void onKeyEvent(int key, int scancode, int action, int mode);
		void onMousePosEvent(float xpos, float ypos);
		void onMouseButtonEvent(int button, int action, int mods);
		void onMouseWheelEvent(float xoffset, float yoffset);

		void setOnKeyEvent(std::function<void(int, int)> callback) { onKeyEventCallback = callback; }
		void setOnMousePosEvent(std::function<void(float, float)> callback) { onMousePosEventCallback = callback; }
		void setOnMouseButtonEvent(std::function<void(int, int)> callback) { onMouseButtonEventCallback = callback; }
		void setOnMouseWheelEvent(std::function<void(float, float)> callback) { onMouseWheelEventCallback = callback; }
	};
}