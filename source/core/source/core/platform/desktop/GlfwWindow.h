#pragma once

#include <functional>
#include <iostream>
#include <string>

#include <core/gl/api.h>
#include <core/input/CharEvent.h>
#include <core/input/InputListener.h>
#include <core/input/KeyEvent.h>
#include <core/input/MousePosEvent.h>

#include "../IWindow.h"
#include "File.h"

class GLFWwindow;

namespace yage::platform::desktop
{
    class GlfwWindow final : public IWindow, public std::enable_shared_from_this<GlfwWindow>
    {
    public:
        GlfwWindow(int width, int height, const std::string& title, gl::Api gl_api);

        ~GlfwWindow() override;

        std::weak_ptr<gl::IContext> gl_context() override;

        void makeCurrent() override;
        void pollEvents() override;
        void swapBuffers() override;

        void show() override;

        void hide() override;

        [[nodiscard]] std::unique_ptr<IFileReader> getFileReader() override;

        [[nodiscard]] int width() const override;

        [[nodiscard]] int height() const override;

        [[nodiscard]] int pixel_width() const override;

        [[nodiscard]] int pixel_height() const override;

        void enableVSync();
        void disableVSync();

        void toggleCursor() override;
        void hideCursor() override;
        void showCursor() override;

        int should_destroy() override;

        void enableCharInput() override;
        void disableCharInput() override;
        void enableKeyInput() override;
        void disableKeyInput() override;

        [[nodiscard]] int dpi() const override
        {
            return m_dpi;
        }

        [[nodiscard]] double getTime() const override;
        double getTimeStep() override;

        void log(const std::string& msg);

        std::string openFileDialog(std::string defaultPath) override;
        std::string openFileDialog(std::string defaultPath = "", std::vector<std::string> filterList = {},
                                   std::string filterName = "");

        void attach_on_resize(std::function<void(int, int)> callback) override;

        void attach(input::InputListener& listener) override;
        void detach(input::InputListener& listener);

        static void onCharEvent(GLFWwindow* window, unsigned int codepoint);
        static void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mode);
        static void onMousePosEvent(GLFWwindow* window, double xPos, double yPos);
        static void onMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);
        static void onMouseWheelEvent(GLFWwindow* window, double xOffset, double yOffset);
        static void on_framebuffer_resize_event(GLFWwindow* window, int width, int height);

        static void onError(int error, const char* description);

        GLFWwindow* glfw_window_ptr(); // TODO: ugly

    private:
        GLFWwindow* m_glfw_handle = nullptr;
        std::shared_ptr<gl::IContext> m_gl_context;

        std::vector<std::reference_wrapper<input::InputListener>> m_input_listeners;
        std::vector<std::function<void(int, int)>> m_on_resize_callbacks;

        bool m_cursor_visible = true;
        bool m_char_input_enabled = false;
        bool m_key_input_enabled = true;
        int m_dpi = 0;
        double m_last_time_step = 0.0;

        void notifyListeners(const input::KeyEvent& event);
        void notifyListeners(const input::MousePosEvent& event);
        void notifyListeners(const input::MouseWheelEvent& event);
        void notifyListeners(const input::CharEvent& event);

        static input::KeyEvent::Code convertToKeyCode(int key) noexcept;
        static input::KeyEvent::Action convertToKeyAction(int action) noexcept;
        static input::KeyEvent::Code convertToMouseButtonCode(int button) noexcept;
    };
}
