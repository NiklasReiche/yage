#pragma once

#include <memory>
#include <string>
#include <functional>

#include <core/gl/IContext.h>
#include <core/input/InputListener.h>
#include "IFileReader.h"


namespace yage::platform
{
	class IWindow
	{
	public:
		virtual ~IWindow() = default;

	    virtual std::weak_ptr<gl::IContext> gl_context() = 0;

		virtual void show() = 0;

		virtual void hide() = 0;

	    virtual int should_destroy() = 0;

		virtual void makeCurrent() = 0;
		virtual void pollEvents() = 0;
		virtual void swapBuffers() = 0;

		virtual void attach(input::InputListener & listener) = 0;

        virtual void attach_on_resize(std::function<void(int, int)>) = 0;

	    virtual void toggleCursor() = 0;
        virtual void hideCursor() = 0;
        virtual void showCursor() = 0;

		virtual void enableCharInput() = 0;
		virtual void disableCharInput() = 0;
		virtual void enableKeyInput() = 0;
		virtual void disableKeyInput() = 0;

        [[nodiscard]]
        virtual std::unique_ptr<IFileReader> getFileReader() = 0;

		virtual std::string openFileDialog(std::string defaultPath) = 0;

		[[nodiscard]]
		virtual double getTime() const = 0;

		/**
		 * @return The time since the last call to this method in seconds
		 */
		virtual double getTimeStep() = 0;

		[[nodiscard]] virtual int width() const = 0;

	    [[nodiscard]] virtual int height() const = 0;

	    [[nodiscard]] virtual int pixel_width() const = 0;

		[[nodiscard]] virtual int pixel_height() const = 0;

		[[nodiscard]] virtual int dpi() const = 0;
	};
}
