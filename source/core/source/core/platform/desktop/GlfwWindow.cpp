#include "GlfwWindow.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <tfd/tinyfiledialogs.h>

#include <utils/strings.h>
#include <utils/NotImplementedException.h>

#include "GlfwException.h"
#include "FileReader.h"

namespace platform::desktop
{
	GlfwWindow::GlfwWindow(int width, int height, const std::string& title)
	{
		if (width < 1 || height < 1) {
			throw std::invalid_argument("Window dimensions must be positive");
		}

		glfwSetErrorCallback(onError);

		if (!glfwInit()) {
			throw GlfwException(-1, "Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		glfwWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (glfwWindow == nullptr) {
			glfwTerminate();
			throw GlfwException(-1, "Failed to create GLFW window");
		}

		glfwSetWindowUserPointer(glfwWindow, this);

		glfwMakeContextCurrent(glfwWindow);
		glfwSwapInterval(1);

		glfwSetCharCallback(glfwWindow, onCharEvent);
		glfwSetKeyCallback(glfwWindow, onKeyEvent);
		glfwSetCursorPosCallback(glfwWindow, onMousePosEvent);
		glfwSetMouseButtonCallback(glfwWindow, onMouseButtonEvent);
		glfwSetScrollCallback(glfwWindow, onMouseWheelEvent);

		dpi = 96;
	}
	
	GlfwWindow::~GlfwWindow()
	{
		glfwTerminate();
	}

	void GlfwWindow::show()
	{
		glfwShowWindow(glfwWindow);
	}

	void GlfwWindow::hide()
	{
		glfwHideWindow(glfwWindow);
	}

	int GlfwWindow::getWidth() const
	{
		int width;
		glfwGetWindowSize(glfwWindow, &width, nullptr);
		return width;
	}

	int GlfwWindow::getHeight() const
	{
		int height;
		glfwGetWindowSize(glfwWindow, nullptr, &height);
		return height;
	}

	int GlfwWindow::getPixelWidth() const
	{
	    int width;
		glfwGetFramebufferSize(glfwWindow, &width, nullptr);
		return width;
	}

	int GlfwWindow::getPixelHeight() const
	{
		int height;
		glfwGetFramebufferSize(glfwWindow, nullptr, &height);
		return height;
	}

	void GlfwWindow::makeCurrent()
	{
		glfwMakeContextCurrent(glfwWindow);
	}

	void GlfwWindow::pollEvents()
	{
		glfwPollEvents();
	}

	void GlfwWindow::swapBuffers()
	{
		glfwSwapBuffers(glfwWindow);
	}

	void GlfwWindow::enableVSync()
	{
		glfwSwapInterval(1);
	}

	void GlfwWindow::disableVSync()
	{
		glfwSwapInterval(0);
	}

	void GlfwWindow::hideCursor()
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void GlfwWindow::showCursor()
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	int GlfwWindow::shouldDestroy()
	{
		return glfwWindowShouldClose(glfwWindow);
	}

	void GlfwWindow::enableCharInput()
	{
		isCharInputEnabled = true;
	}
	void GlfwWindow::disableCharInput()
	{
		isCharInputEnabled = false;
	}
	void GlfwWindow::enableKeyInput()
	{
		isKeyInputEnabled = true;
	}
	void GlfwWindow::disableKeyInput()
	{
		isKeyInputEnabled = false;
	}

	double GlfwWindow::getTime() const
	{
		return glfwGetTime();
	}
	double GlfwWindow::getTimeStep()
	{
		const double now = glfwGetTime();
		const double step = now - lastTimeStep;
		lastTimeStep = now;
		return step;
	}


	void GlfwWindow::attach(input::InputListener& listener)
	{
		inputListeners.emplace_back(listener);
	}

	void GlfwWindow::detach(input::InputListener&)
	{
		throw NotImplementedException();
	}

	void GlfwWindow::notifyListeners(const input::KeyEvent& event)
	{
		for (auto listener : inputListeners)
		{
			listener.get().onKeyEvent(event);
		}
	}
	void GlfwWindow::notifyListeners(const input::MousePosEvent& event)
	{
		for (auto listener : inputListeners)
		{
			listener.get().onMousePosEvent(event);
		}
	}
	void GlfwWindow::notifyListeners(const input::MouseWheelEvent& event)
	{
		for (auto listener : inputListeners)
		{
			listener.get().onMouseWheelEvent(event);
		}
	}

	void GlfwWindow::onCharEvent(GLFWwindow* window, const unsigned int)
	{
		auto handle = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		if (!handle->isCharInputEnabled) {
			return;
		}
		throw NotImplementedException();
	}
	void GlfwWindow::onKeyEvent(GLFWwindow* window, const int key, const int, const int action, const int)
	{
		auto handle = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		const input::KeyEvent event(convertToKeyCode(key), convertToKeyAction(action));
		handle->notifyListeners(event);
	}
	void GlfwWindow::onMousePosEvent(GLFWwindow* window, const double xPos, const double yPos)
	{
		auto handle = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		const input::MousePosEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
		handle->notifyListeners(event);
	}
	void GlfwWindow::onMouseButtonEvent(GLFWwindow* window, const int button, const int action, const int)
	{
		auto handle = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		const input::KeyEvent event(convertToMouseButtonCode(button), convertToKeyAction(action));
		handle->notifyListeners(event);
	}
	void GlfwWindow::onMouseWheelEvent(GLFWwindow* window, const double xOffset, const double yOffset)
	{
		auto handle = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
		const input::MouseWheelEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
		handle->notifyListeners(event);
	}

	void GlfwWindow::onError(const int error, const char* description)
	{
		throw GlfwException(error, std::string(description));
	}

	void GlfwWindow::log(const std::string& msg)
	{
		std::cout << msg << std::endl;
	}
	
	std::string GlfwWindow::openFileDialog(std::string defaultPath, std::vector<std::string> filterList, std::string filterName)
	{
		const char* defaultPathC = nullptr;
		if (!defaultPath.empty()) {
			defaultPathC = utils::replaceAll(defaultPath, "\\", "/").c_str();
		}

		std::vector<const char*> filterListC;
		for (unsigned int i = 0; i < filterList.size(); ++i) {
			filterListC.push_back(filterList.at(i).c_str());
		}

		const char* result = tinyfd_openFileDialog("Open File", defaultPathC, filterListC.size(), &filterListC[0], filterName.c_str(), 0);

		if (result == nullptr) {
			return "";
		}
		else {
			return std::string(result);
		}
	}

	input::KeyEvent::Code GlfwWindow::convertToKeyCode(const int key) noexcept
	{
		switch (key)
		{
			case GLFW_KEY_UNKNOWN: return input::KeyEvent::Code::KEY_UNKNOWN;
			case GLFW_KEY_SPACE: return input::KeyEvent::Code::KEY_SPACE;
			case GLFW_KEY_APOSTROPHE: return input::KeyEvent::Code::KEY_APOSTROPHE;
			case GLFW_KEY_COMMA: return input::KeyEvent::Code::KEY_COMMA;
			case GLFW_KEY_MINUS: return input::KeyEvent::Code::KEY_MINUS;
			case GLFW_KEY_PERIOD: return input::KeyEvent::Code::KEY_PERIOD;
			case GLFW_KEY_SLASH: return input::KeyEvent::Code::KEY_SLASH;
			case GLFW_KEY_0: return input::KeyEvent::Code::KEY_0;
			case GLFW_KEY_1: return input::KeyEvent::Code::KEY_1;
			case GLFW_KEY_2: return input::KeyEvent::Code::KEY_2;
			case GLFW_KEY_3: return input::KeyEvent::Code::KEY_3;
			case GLFW_KEY_4: return input::KeyEvent::Code::KEY_4;
			case GLFW_KEY_5: return input::KeyEvent::Code::KEY_5;
			case GLFW_KEY_6: return input::KeyEvent::Code::KEY_6;
			case GLFW_KEY_7: return input::KeyEvent::Code::KEY_7;
			case GLFW_KEY_8: return input::KeyEvent::Code::KEY_8;
			case GLFW_KEY_9: return input::KeyEvent::Code::KEY_9;
			case GLFW_KEY_SEMICOLON: return input::KeyEvent::Code::KEY_SEMICOLON;
			case GLFW_KEY_EQUAL: return input::KeyEvent::Code::KEY_EQUAL;
			case GLFW_KEY_A: return input::KeyEvent::Code::KEY_A;
			case GLFW_KEY_B: return input::KeyEvent::Code::KEY_B;
			case GLFW_KEY_C: return input::KeyEvent::Code::KEY_C;
			case GLFW_KEY_D: return input::KeyEvent::Code::KEY_D;
			case GLFW_KEY_E: return input::KeyEvent::Code::KEY_E;
			case GLFW_KEY_F: return input::KeyEvent::Code::KEY_F;
			case GLFW_KEY_H: return input::KeyEvent::Code::KEY_H;
			case GLFW_KEY_I: return input::KeyEvent::Code::KEY_I;
			case GLFW_KEY_J: return input::KeyEvent::Code::KEY_J;
			case GLFW_KEY_K: return input::KeyEvent::Code::KEY_K;
			case GLFW_KEY_L: return input::KeyEvent::Code::KEY_L;
			case GLFW_KEY_M: return input::KeyEvent::Code::KEY_M;
			case GLFW_KEY_N: return input::KeyEvent::Code::KEY_N;
			case GLFW_KEY_O: return input::KeyEvent::Code::KEY_O;
			case GLFW_KEY_P: return input::KeyEvent::Code::KEY_P;
			case GLFW_KEY_Q: return input::KeyEvent::Code::KEY_Q;
			case GLFW_KEY_R: return input::KeyEvent::Code::KEY_R;
			case GLFW_KEY_S: return input::KeyEvent::Code::KEY_S;
			case GLFW_KEY_T: return input::KeyEvent::Code::KEY_T;
			case GLFW_KEY_U: return input::KeyEvent::Code::KEY_U;
			case GLFW_KEY_V: return input::KeyEvent::Code::KEY_V;
			case GLFW_KEY_W: return input::KeyEvent::Code::KEY_W;
			case GLFW_KEY_X: return input::KeyEvent::Code::KEY_X;
			case GLFW_KEY_Y: return input::KeyEvent::Code::KEY_Y;
			case GLFW_KEY_Z: return input::KeyEvent::Code::KEY_Z;
			case GLFW_KEY_LEFT_BRACKET: return input::KeyEvent::Code::KEY_LEFT_BRACKET;
			case GLFW_KEY_BACKSLASH: return input::KeyEvent::Code::KEY_BACKSLASH;
			case GLFW_KEY_RIGHT_BRACKET: return input::KeyEvent::Code::KEY_RIGHT_BRACKET;
			case GLFW_KEY_GRAVE_ACCENT: return input::KeyEvent::Code::KEY_GRAVE_ACCENT;
			case GLFW_KEY_WORLD_1: return input::KeyEvent::Code::KEY_WORLD_1;
			case GLFW_KEY_WORLD_2: return input::KeyEvent::Code::KEY_WORLD_2;
			case GLFW_KEY_ESCAPE: return input::KeyEvent::Code::KEY_ESCAPE;
			case GLFW_KEY_ENTER: return input::KeyEvent::Code::KEY_ENTER;
			case GLFW_KEY_TAB: return input::KeyEvent::Code::KEY_TAB;
			case GLFW_KEY_BACKSPACE: return input::KeyEvent::Code::KEY_BACKSPACE;
			case GLFW_KEY_INSERT: return input::KeyEvent::Code::KEY_INSERT;
			case GLFW_KEY_DELETE: return input::KeyEvent::Code::KEY_DELETE;
			case GLFW_KEY_RIGHT: return input::KeyEvent::Code::KEY_RIGHT;
			case GLFW_KEY_LEFT: return input::KeyEvent::Code::KEY_LEFT;
			case GLFW_KEY_DOWN: return input::KeyEvent::Code::KEY_DOWN;
			case GLFW_KEY_UP: return input::KeyEvent::Code::KEY_UP;
			case GLFW_KEY_PAGE_UP: return input::KeyEvent::Code::KEY_PAGE_UP;
			case GLFW_KEY_PAGE_DOWN: return input::KeyEvent::Code::KEY_PAGE_DOWN;
			case GLFW_KEY_HOME: return input::KeyEvent::Code::KEY_HOME;
			case GLFW_KEY_END: return input::KeyEvent::Code::KEY_END;
			case GLFW_KEY_CAPS_LOCK: return input::KeyEvent::Code::KEY_CAPS_LOCK;
			case GLFW_KEY_SCROLL_LOCK: return input::KeyEvent::Code::KEY_SCROLL_LOCK;
			case GLFW_KEY_NUM_LOCK: return input::KeyEvent::Code::KEY_NUM_LOCK;
			case GLFW_KEY_PRINT_SCREEN: return input::KeyEvent::Code::KEY_PRINT_SCREEN;
			case GLFW_KEY_PAUSE: return input::KeyEvent::Code::KEY_PAUSE;
			case GLFW_KEY_F1: return input::KeyEvent::Code::KEY_F1;
			case GLFW_KEY_F2: return input::KeyEvent::Code::KEY_F2;
			case GLFW_KEY_F3: return input::KeyEvent::Code::KEY_F3;
			case GLFW_KEY_F4: return input::KeyEvent::Code::KEY_F4;
			case GLFW_KEY_F5: return input::KeyEvent::Code::KEY_F5;
			case GLFW_KEY_F6: return input::KeyEvent::Code::KEY_F6;
			case GLFW_KEY_F7: return input::KeyEvent::Code::KEY_F7;
			case GLFW_KEY_F8: return input::KeyEvent::Code::KEY_F8;
			case GLFW_KEY_F9: return input::KeyEvent::Code::KEY_F9;
			case GLFW_KEY_F10: return input::KeyEvent::Code::KEY_F10;
			case GLFW_KEY_F11: return input::KeyEvent::Code::KEY_F11;
			case GLFW_KEY_F12: return input::KeyEvent::Code::KEY_F12;
			case GLFW_KEY_F13: return input::KeyEvent::Code::KEY_F13;
			case GLFW_KEY_F14: return input::KeyEvent::Code::KEY_F14;
			case GLFW_KEY_F15: return input::KeyEvent::Code::KEY_F15;
			case GLFW_KEY_F16: return input::KeyEvent::Code::KEY_F16;
			case GLFW_KEY_F17: return input::KeyEvent::Code::KEY_F17;
			case GLFW_KEY_F18: return input::KeyEvent::Code::KEY_F18;
			case GLFW_KEY_F19: return input::KeyEvent::Code::KEY_F19;
			case GLFW_KEY_F20: return input::KeyEvent::Code::KEY_F20;
			case GLFW_KEY_F21: return input::KeyEvent::Code::KEY_F21;
			case GLFW_KEY_F22: return input::KeyEvent::Code::KEY_F22;
			case GLFW_KEY_F23: return input::KeyEvent::Code::KEY_F23;
			case GLFW_KEY_F24: return input::KeyEvent::Code::KEY_F24;
			case GLFW_KEY_F25: return input::KeyEvent::Code::KEY_F25;
			case GLFW_KEY_KP_0: return input::KeyEvent::Code::KEY_KP_0;
			case GLFW_KEY_KP_1: return input::KeyEvent::Code::KEY_KP_1;
			case GLFW_KEY_KP_2: return input::KeyEvent::Code::KEY_KP_2;
			case GLFW_KEY_KP_3: return input::KeyEvent::Code::KEY_KP_3;
			case GLFW_KEY_KP_4: return input::KeyEvent::Code::KEY_KP_4;
			case GLFW_KEY_KP_5: return input::KeyEvent::Code::KEY_KP_5;
			case GLFW_KEY_KP_6: return input::KeyEvent::Code::KEY_KP_6;
			case GLFW_KEY_KP_7: return input::KeyEvent::Code::KEY_KP_7;
			case GLFW_KEY_KP_8: return input::KeyEvent::Code::KEY_KP_8;
			case GLFW_KEY_KP_9: return input::KeyEvent::Code::KEY_KP_9;
			case GLFW_KEY_KP_DECIMAL: return input::KeyEvent::Code::KEY_KP_DECIMAL;
			case GLFW_KEY_KP_DIVIDE: return input::KeyEvent::Code::KEY_KP_DIVIDE;
			case GLFW_KEY_KP_MULTIPLY: return input::KeyEvent::Code::KEY_KP_MULTIPLY;
			case GLFW_KEY_KP_SUBTRACT: return input::KeyEvent::Code::KEY_KP_SUBTRACT;
			case GLFW_KEY_KP_ADD: return input::KeyEvent::Code::KEY_KP_ADD;
			case GLFW_KEY_KP_ENTER: return input::KeyEvent::Code::KEY_KP_ENTER;
			case GLFW_KEY_KP_EQUAL: return input::KeyEvent::Code::KEY_KP_EQUAL;
			case GLFW_KEY_LEFT_SHIFT: return input::KeyEvent::Code::KEY_LEFT_SHIFT;
			case GLFW_KEY_LEFT_CONTROL: return input::KeyEvent::Code::KEY_LEFT_CONTROL;
			case GLFW_KEY_LEFT_ALT: return input::KeyEvent::Code::KEY_LEFT_ALT;
			case GLFW_KEY_LEFT_SUPER: return input::KeyEvent::Code::KEY_LEFT_SUPER;
			case GLFW_KEY_RIGHT_SHIFT: return input::KeyEvent::Code::KEY_RIGHT_SHIFT;
			case GLFW_KEY_RIGHT_CONTROL: return input::KeyEvent::Code::KEY_RIGHT_CONTROL;
			case GLFW_KEY_RIGHT_ALT: return input::KeyEvent::Code::KEY_RIGHT_ALT;
			case GLFW_KEY_RIGHT_SUPER: return input::KeyEvent::Code::KEY_RIGHT_SUPER;
			case GLFW_KEY_MENU: return input::KeyEvent::Code::KEY_MENU;
			default: return input::KeyEvent::Code::KEY_UNKNOWN;
		}
	}

	input::KeyEvent::Action GlfwWindow::convertToKeyAction(const int action) noexcept
	{
		switch (action)
		{
			case GLFW_PRESS: return input::KeyEvent::Action::PRESS;
			case GLFW_RELEASE: return input::KeyEvent::Action::RELEASE;
			case GLFW_REPEAT: return input::KeyEvent::Action::REPEAT;
			default: return input::KeyEvent::Action::UNKNOWN;
		}
	}

	input::KeyEvent::Code GlfwWindow::convertToMouseButtonCode(const int button) noexcept
	{
		switch (button)
		{
			case GLFW_MOUSE_BUTTON_1: return input::KeyEvent::Code::KEY_MOUSE_1;
			case GLFW_MOUSE_BUTTON_2: return input::KeyEvent::Code::KEY_MOUSE_2;
			case GLFW_MOUSE_BUTTON_3: return input::KeyEvent::Code::KEY_MOUSE_3;
			case GLFW_MOUSE_BUTTON_4: return input::KeyEvent::Code::KEY_MOUSE_4;
			case GLFW_MOUSE_BUTTON_5: return input::KeyEvent::Code::KEY_MOUSE_5;
			case GLFW_MOUSE_BUTTON_6: return input::KeyEvent::Code::KEY_MOUSE_6;
			case GLFW_MOUSE_BUTTON_7: return input::KeyEvent::Code::KEY_MOUSE_7;
			case GLFW_MOUSE_BUTTON_8: return input::KeyEvent::Code::KEY_MOUSE_8;
			default: return input::KeyEvent::Code::KEY_UNKNOWN;
		}
	}

    std::unique_ptr<platform::IFileReader> GlfwWindow::getFileReader()
    {
        return std::make_unique<FileReader>(FileReader());
    }
}
