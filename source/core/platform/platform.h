#pragma once

#ifdef DESKTOP
#include "desktop/platformHandle_desktop.h"
#include "desktop/file_desktop.h"
#endif

#ifdef ANDROID
#include "android/Android_Handle.h"
#include "android/Android_File.h"
#endif

#include "util.h"
#include "exception.h"

namespace sys
{
#ifdef DESKTOP
	typedef desktop::PlatformHandle PlatformHandle;
	typedef desktop::File File;
	typedef desktop::SeekOffset SeekOffset;
	typedef desktop::AccessMode AccessMode;
#endif

#ifdef ANDROID
	typedef android::Android_Handle PlatformHandle;
	typedef android::Android_File File;
	typedef android::SeekOffset SeekOffset;
#endif
}