#pragma once

#ifdef DESKTOP
#include "desktop/GLFW_Handle.h"
#endif

#ifdef ANDROID
#include "android/Android_Handle.h"
#endif

namespace platform
{
#ifdef DESKTOP
	typedef glfw::GLFWHandle PlatformHandle;
#endif

#ifdef ANDROID
	typedef android::Android_Handle PlatformHandle;
#endif
}