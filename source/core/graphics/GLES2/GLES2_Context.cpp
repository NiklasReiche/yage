#include "GLES2_Context.h"

namespace gles2
{
	void GLES2_Context::checkError(std::string loc)
	{
		EGLint error = eglGetError();
		if (error != EGL_SUCCESS) {
			std::stringstream str;
			str << "\n EGL::ERROR at " << loc << ": " << error << "\n";
			std::string tmp = str.str();
			const char* msg = tmp.c_str();
			__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", msg);
		}
	}


	GLES2_Context::GLES2_Context(android::AndroidApp* app)
		: app(app)
	{
		app->log("GLES2-Context entry");

		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};

		eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		checkError("eglDisplay()");
		app->log("eglDisplay");

		eglInitialize(eglDisplay, 0, 0);
		checkError("eglInitialize()");
		app->log("eglInitialize(");

		EGLint numConfigs;
		eglChooseConfig(eglDisplay, attribs, nullptr, 0, &numConfigs);
		checkError("eglChooseConfig");
		app->log("eglChooseConfig 1");

		std::vector<EGLConfig> configs(numConfigs);
		eglChooseConfig(eglDisplay, attribs, &configs[0], numConfigs, &numConfigs);
		checkError("eglChooseConfig");
		app->log("eglChooseConfig 2");

		EGLConfig config;
		int i = 0;
		for (; i < numConfigs; ++i)
		{
			int r, g, b, d;
			eglGetConfigAttrib(eglDisplay, configs[i], EGL_RED_SIZE, &r);
			eglGetConfigAttrib(eglDisplay, configs[i], EGL_RED_SIZE, &g);
			eglGetConfigAttrib(eglDisplay, configs[i], EGL_RED_SIZE, &b);
			eglGetConfigAttrib(eglDisplay, configs[i], EGL_DEPTH_SIZE, &d);

			if (r == 8 && g == 8 && b == 8 && d == 0) {
				config = configs[i];
			}
			else if (i == numConfigs - 1) {
				config = configs[0];
			}
		}

		eglSurface = eglCreateWindowSurface(eglDisplay, config, app->getWindow(), NULL);
		checkError("eglCreateWindowSurface");
		app->log("eglCreateWindowSurface");

		eglContext = eglCreateContext(eglDisplay, config, NULL, NULL);
		checkError("eglCreateContext");
		eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
		checkError("eglMakeCurrent");

		eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &width);
		eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &height);
	}
	GLES2_Context::~GLES2_Context()
	{
		eglTerminate(eglDisplay);
	}

	void GLES2_Context::makeCurrent()
	{
		eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	}
	void GLES2_Context::swapBuffers()
	{
		eglSwapBuffers(eglDisplay, eglSurface);
	}

	void GLES2_Context::clear()
	{
		glClearColor(1.0f, 1.0f, 0.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}