#pragma once

#include <functional>

namespace sys
{
	class GenericPlatformHandle
	{
	public:
		GenericPlatformHandle() {}
		virtual ~GenericPlatformHandle() {}

		virtual void makeCurrent() {}
		virtual void pollEvents() {}
		virtual void swapBuffers() {}

		virtual int shouldDestroy() { return 0; }

		virtual void enableCharInput() {}
		virtual void disableCharInput() {}

		virtual double getTime() { return 0; }

		virtual void setOnCharModsEvent(std::function<void(unsigned int, int)> callback) {}
		virtual void setOnKeyEvent(std::function<void(int, int)> callback) {}
		virtual void setOnMousePosEvent(std::function<void(float, float)> callback) {}
		virtual void setOnMouseButtonEvent(std::function<void(int, int)> callback) {}
		virtual void setOnMouseWheelEvent(std::function<void(float, float)> callback) {}
		virtual void setOnTouchEvent(std::function<void(float, float, int, int)>) {}
	};
}