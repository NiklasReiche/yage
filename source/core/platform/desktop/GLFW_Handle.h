#pragma once

#include <functional>

#include <GLFW\glfw3.h>
#include "GLFW_Callback.h"
#include "Keyboard.h"

#include <platform\GenericHandle.h>

namespace glfw
{
	class GLFWHandle : public platform::GenericPlatformHandle
	{
	private:
		GLFWwindow* glfwWindow;

		std::function<void(KeyCode, KeyAction)> onKeyEventCallback;
		std::function<void(double, double)> onMousePosEventCallback;
		std::function<void(MouseKeyCode, KeyAction)> onMouseButtonEventCallback;
		std::function<void(double, double)> onMouseWheelEventCallback;

	public:
		GLFWHandle();
		~GLFWHandle();

		void createContext(int width, int height, std::string title = "OpenGL", int versionMajor = 3, int versionMinor = 3);

		void showWindow();
		void hideWindow();
		void makeCurrent();
		void pollEvents();
		void swapBuffers();

		int shouldDestroy();

		void onKeyEvent(int key, int scancode, int action, int mode);
		void onMousePosEvent(double xpos, double ypos);
		void onMouseButtonEvent(int button, int action, int mods);
		void onMouseWheelEvent(double xoffset, double yoffset);

		void setOnKeyEvent(std::function<void(KeyCode, KeyAction)> callback) { onKeyEventCallback = callback; }
		void setOnMousePosEvent(std::function<void(double, double)> callback) { onMousePosEventCallback = callback; }
		void setOnMouseButtonEvent(std::function<void(MouseKeyCode, KeyAction)> callback) { onMouseButtonEventCallback = callback; }
		void setOnMouseWheelEvent(std::function<void(double, double)> callback) { onMouseWheelEventCallback = callback; }
	};
}