#pragma once

#define DESKTOP

#ifdef DESKTOP
#include "desktop\GLFW_Handle.h"
#endif

#ifdef ANDROID
#include "android\AndroidApp.h"
#endif

#include "desktop\Keyboard.h"

namespace platform
{
#ifdef DESKTOP
	typedef glfw::GLFWHandle PlatformHandle;
#endif

#ifdef ANDROID
	typedef android::AndroidApp PlatformHandle;
#endif

	typedef glfw::KeyAction KeyAction;
	typedef glfw::KeyCode KeyCode;
	typedef glfw::MouseKeyCode MouseKeyCode;
	typedef glfw::JoystickKeyCode JoystickKeyCode;
}