#include "Android_Handle.h"

namespace android
{
	void handleCommand(struct android_app* app, int32_t cmd)
	{
		Android_Handle* androidHandle = (Android_Handle*)app->userData;
		try {
			switch (cmd)
			{
			case APP_CMD_INPUT_CHANGED:
				break;

			case APP_CMD_INIT_WINDOW:
				if (app->window != NULL) {
					androidHandle->callbacks.onWindowInit();
				}
				break;

			case APP_CMD_TERM_WINDOW:
				androidHandle->callbacks.onWindowTerm();
				break;

			case APP_CMD_WINDOW_RESIZED:
				break;

			case APP_CMD_WINDOW_REDRAW_NEEDED:
				break;

			case APP_CMD_CONTENT_RECT_CHANGED:
				break;

			case APP_CMD_GAINED_FOCUS:
				break;

			case APP_CMD_LOST_FOCUS:
				break;

			case APP_CMD_CONFIG_CHANGED:
				break;

			case APP_CMD_LOW_MEMORY:
				break;

			case APP_CMD_START:
				androidHandle->callbacks.onAppStart();
				break;

			case APP_CMD_RESUME:
				androidHandle->callbacks.onAppResume();
				break;

			case APP_CMD_SAVE_STATE:
				break;

			case APP_CMD_PAUSE:
				androidHandle->callbacks.onAppPause();
				break;

			case APP_CMD_STOP:
				androidHandle->callbacks.onAppStop();
				break;

			case APP_CMD_DESTROY:
				androidHandle->callbacks.onAppDestroy();
				break;
			}
		}
		catch (std::bad_function_call) {}
	}
	int32_t handleInputEvent(struct android_app* app, AInputEvent* event)
	{
		Android_Handle* androidHandle = (Android_Handle*)app->userData;

		int32_t type = AInputEvent_getType(event);
		if (type == AINPUT_EVENT_TYPE_KEY) {

		} 
		else if (type == AINPUT_EVENT_TYPE_MOTION) {
			int32_t action = AMotionEvent_getAction(event);
			int32_t pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
			int32_t actionIdent = action & AMOTION_EVENT_ACTION_MASK;

			float x = AMotionEvent_getRawX(event, 0);
			float y = AMotionEvent_getRawY(event, 0);

			try {
				androidHandle->callbacks.onTouchEvent(x, y, pointerIndex, actionIdent);
			}
			catch (std::bad_function_call) {}
			return 1;
		}
		return 0;
	}

	void Android_Handle::checkError(std::string msg)
	{
		EGLint error = eglGetError();
		if (error != EGL_SUCCESS) {
			std::stringstream str;
			str << "\n EGL::ERROR: " << msg << ": " << error << "\n";
			std::string tmp = str.str();
			const char* output = tmp.c_str();
			__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "%s", output);
		}
	}

	Android_Handle::Android_Handle(android_app* app)
		: app(app)
	{
		app_dummy();
		app->userData = this;
		app->onAppCmd = handleCommand;
		app->onInputEvent = handleInputEvent;
	}
	Android_Handle::~Android_Handle()
	{
		eglTerminate(eglDisplay);
	}

	void Android_Handle::createContext(int * width, int * height, bool useGles3)
	{
		/* wait until the app window is ready*/
		while (app->window == NULL)
		{
			pollEvents();
		}

		int glVersion = EGL_OPENGL_ES2_BIT;
		if (useGles3) {
			//glVersion = EGL_OPENGL_ES3_BIT_KHR;
		}
		const EGLint attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, glVersion,
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
		};

		eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		checkError("eglDisplay()");

		eglInitialize(eglDisplay, 0, 0);
		checkError("eglInitialize()");

		EGLint numConfigs;
		eglChooseConfig(eglDisplay, attribs, nullptr, 0, &numConfigs);
		checkError("eglChooseConfig()");

		std::vector<EGLConfig> configs(numConfigs);
		eglChooseConfig(eglDisplay, attribs, &configs[0], numConfigs, &numConfigs);
		checkError("eglChooseConfig()");

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

		eglSurface = eglCreateWindowSurface(eglDisplay, config, app->window, NULL);
		checkError("eglCreateWindowSurface");

		eglContext = eglCreateContext(eglDisplay, config, NULL, NULL);
		checkError("eglCreateContext");

		eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
		checkError("eglMakeCurrent");

		eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, width);
		eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, height);
	}

	void Android_Handle::makeCurrent()
	{
		eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	}
	void Android_Handle::swapBuffers()
	{
		eglSwapBuffers(eglDisplay, eglSurface);
	}
	void Android_Handle::pollEvents()
	{
		int events;
		android_poll_source* source;

		while (ALooper_pollAll(0, NULL, &events, (void**)&source) >= 0)
		{
			if (source != NULL) {
				source->process(app, source);
			}
		}
	}

	int Android_Handle::shouldDestroy()
	{
		return app->destroyRequested;
	}

	void Android_Handle::log(std::string msg, std::string logTag)
	{
		std::stringstream str;
		str << "\n" << msg << "\n";
		std::string tmp = str.str();
		const char* cmsg = tmp.c_str();
		__android_log_print(ANDROID_LOG_DEBUG, logTag.c_str(), "%s", cmsg);
	}
}