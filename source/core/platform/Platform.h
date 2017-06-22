#pragma once

#ifdef DESKTOP
#include "desktop/GLFW_Handle.h"
#include "desktop/Desktop_File.h"
#endif

#ifdef ANDROID
#include "android/Android_Handle.h"
#include "android/Android_File.h"
#endif

#include "android/clib.h"

namespace platform
{
#ifdef DESKTOP
	typedef glfw::GLFWHandle PlatformHandle;
	typedef glfw::Desktop_File File;
	typedef glfw::SeekOffset SeekOffset;
#endif

#ifdef ANDROID
	typedef android::Android_Handle PlatformHandle;
	typedef android::Android_File File;
	typedef android::SeekOffset SeekOffset;
#endif
}