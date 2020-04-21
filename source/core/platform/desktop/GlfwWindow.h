#pragma once

#include <string>
#include <iostream>
#include <functional>

#include "../OpenGL.h"

#include <graphics/Context.h>

#include "../Window.h"
#include "File.h"
#include <input/InputListener.h>
#include <input/KeyEvent.h>
#include <input/MousePosEvent.h>

namespace sys::desktop
{
	class GlfwWindow final : public IWindow, public std::enable_shared_from_this<GlfwWindow>
	{
	public:
		GlfwWindow();
		~GlfwWindow();

		std::shared_ptr<gl::IContext> createContext(
			int width, 
			int height, 
			const std::string & title = "OpenGL", 
			int versionMajor = 4, 
			int versionMinor = 3);

		void makeCurrent() override;
		void pollEvents() override;
		void swapBuffers() override;
		void showWindow();
		void hideWindow();

		[[nodiscard]]
		int getWidth() const override;
		[[nodiscard]]
		int getHeight() const override;
		[[nodiscard]]
		std::string getTitle() const override;

		void enableVSync();
		void disableVSync();

		void hideCursor();
		void showCursor();

		int shouldDestroy() override;

		void enableCharInput() override;
		void disableCharInput() override;
		void enableKeyInput() override;
		void disableKeyInput() override;

		[[nodiscard]]
		int getDpi() const override { return dpi; }
		[[nodiscard]]
		double getTime() const override;
		double getTimeStep();

		void log(const std::string & msg);
		std::string openFileDialog(std::string defaultPath = "", std::vector<std::string> filterList = {}, std::string filterName = "");

		
		void attach(input::InputListener & listener) override;
		void detach(input::InputListener & listener);

		static void onCharEvent(GLFWwindow* window, unsigned int codepoint);
		static void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode);
		static void onMousePosEvent(GLFWwindow* window, double xPos, double yPos);
		static void onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
		static void onMouseWheelEvent(GLFWwindow* window, double xOffset, double yOffset);

		static void onError(int error, const char* description);

	private:
		GLFWwindow* glfwWindow = nullptr;

		std::vector<std::reference_wrapper<input::InputListener>> inputListeners;

		bool isCharInputEnabled = false;
		bool isKeyInputEnabled = true;
		int dpi = 0;
		double lastTimeStep = 0.0;

		void notifyListeners(const input::KeyEvent& event);
		void notifyListeners(const input::MousePosEvent& event);
		void notifyListeners(const input::MouseWheelEvent& event);

		static input::KeyEvent::Code convertToKeyCode(int key) noexcept;
		static input::KeyEvent::Action convertToKeyAction(int action) noexcept;
		static input::KeyEvent::Code convertToMouseButtonCode(int button) noexcept;
	};
}
