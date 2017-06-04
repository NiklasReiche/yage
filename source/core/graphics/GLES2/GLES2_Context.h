#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "gles2.h"

#include <platform/android/AndroidApp.h>

namespace gles2
{
	class GLES2_Context
	{
	private:
		EGLDisplay eglDisplay;
		EGLSurface eglSurface;
		EGLContext eglContext;
		int width, height;

		android::AndroidApp* app;

		void checkError(std::string loc);
		
	public:
		GLES2_Context(android::AndroidApp* app);
		~GLES2_Context();

		void makeCurrent();
		void swapBuffers();

		void clear();
	};
}