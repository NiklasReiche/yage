#pragma once

#include <memory>
#include <string>

#include <core/input/InputListener.h>
#include "IFileReader.h"

namespace yage::platform
{
	class IWindow;
	typedef std::shared_ptr<IWindow> Window;

	class IWindow
	{
	public:
		virtual ~IWindow() = default;

		virtual void show() = 0;
		virtual void hide() = 0;

		virtual void makeCurrent() = 0;
		virtual void pollEvents() = 0;
		virtual void swapBuffers() = 0;

		virtual void attach(input::InputListener & listener) = 0;

		virtual int shouldDestroy() = 0;

		virtual void enableCharInput() = 0;
		virtual void disableCharInput() = 0;
		virtual void enableKeyInput() = 0;
		virtual void disableKeyInput() = 0;

        [[nodiscard]]
        virtual std::unique_ptr<IFileReader> getFileReader() = 0;

		[[nodiscard]]
		virtual double getTime() const = 0;

		/**
		 * @return The time since the last call to this method in seconds
		 */
		virtual double getTimeStep() = 0;

		[[nodiscard]]
		virtual int getWidth() const = 0;
		[[nodiscard]]
		virtual int getHeight() const = 0;
		[[nodiscard]]
		virtual int getPixelWidth() const = 0;
		[[nodiscard]]
		virtual int getPixelHeight() const = 0;

		[[nodiscard]]
		virtual int getDpi() const = 0;
	};
}
