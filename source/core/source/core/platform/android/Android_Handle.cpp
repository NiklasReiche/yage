#include "Android_Handle.h"

namespace android
{
	void handleCommand(struct android_app* app, int32_t cmd)
	{
		Android_Handle* androidHandle = (Android_Handle*)app->userData;
		switch (cmd)
		{
		case APP_CMD_INPUT_CHANGED:
			break;

		case APP_CMD_INIT_WINDOW:
			if (app->window != NULL && androidHandle->callbacks.onWindowInit) {
				androidHandle->callbacks.onWindowInit();
			}
			break;

		case APP_CMD_TERM_WINDOW:
			if (androidHandle->callbacks.onWindowTerm) {
				androidHandle->callbacks.onWindowTerm();
			}
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
			if (androidHandle->callbacks.onAppStart) {
				androidHandle->callbacks.onAppStart();
			}
			break;

		case APP_CMD_RESUME:
			if (androidHandle->callbacks.onAppResume) {
				androidHandle->callbacks.onAppResume();
			}
			break;

		case APP_CMD_SAVE_STATE:
			break;

		case APP_CMD_PAUSE:
			if (androidHandle->callbacks.onAppPause) {
				androidHandle->callbacks.onAppPause();
			}
			break;

		case APP_CMD_STOP:
			if (androidHandle->callbacks.onAppStop) {
				androidHandle->callbacks.onAppStop();
			}
			break;

		case APP_CMD_DESTROY:
			if (androidHandle->callbacks.onAppDestroy) {
				androidHandle->callbacks.onAppDestroy();
			}
			break;
		}
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

			if (androidHandle->callbacks.onTouchEvent) {
				androidHandle->callbacks.onTouchEvent(x, y, pointerIndex, actionIdent);
			}
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


		/* copied from stackoverflow - working, but no idea how exactly */
		app->activity->vm->AttachCurrentThread(&jni, NULL);
		jclass activityClass = jni->FindClass("android/app/NativeActivity");
		jmethodID getWindowManager = jni->GetMethodID(activityClass, "getWindowManager", "()Landroid/view/WindowManager;");
		jobject wm = jni->CallObjectMethod(app->activity->clazz, getWindowManager);
		jclass windowManagerClass = jni->FindClass("android/view/WindowManager");
		jmethodID getDefaultDisplay = jni->GetMethodID(windowManagerClass, "getDefaultDisplay", "()Landroid/view/Display;");
		jobject display = jni->CallObjectMethod(wm, getDefaultDisplay);
		jclass displayClass = jni->FindClass("android/view/Display");
		jclass displayMetricsClass = jni->FindClass("android/util/DisplayMetrics");
		jmethodID displayMetricsConstructor = jni->GetMethodID(displayMetricsClass, "<init>", "()V");
		jobject displayMetrics = jni->NewObject(displayMetricsClass, displayMetricsConstructor);
		jmethodID getMetrics = jni->GetMethodID(displayClass, "getMetrics", "(Landroid/util/DisplayMetrics;)V");
		jni->CallVoidMethod(display, getMetrics, displayMetrics);
		jfieldID xdpi_id = jni->GetFieldID(displayMetricsClass, "xdpi", "F");
		dpi = jni->GetFloatField(displayMetrics, xdpi_id);


		assetManager = app->activity->assetManager;

		const char* externalDir = app->activity->externalDataPath;
		const char* internalDir = app->activity->internalDataPath;
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
			EGL_DEPTH_SIZE, 16,
			EGL_STENCIL_SIZE, 8,
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

		EGLint contextAttribs[] =
		{
			EGL_CONTEXT_CLIENT_VERSION, 2, // Specifies OpenGL ES 2.0.
			EGL_NONE
		};
		eglContext = eglCreateContext(eglDisplay, config, NULL, contextAttribs);
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

	double Android_Handle::getTime()
	{
		struct timespec time;
		clock_gettime(CLOCK_REALTIME, &time);
		return (double)time.tv_sec + time.tv_nsec / 1000000000.0;
	}
	double Android_Handle::getTimeStep()
	{
		double now = getTime();
		double step = now - lastTimeStep;
		lastTimeStep = now;
		return step;
	}

	void Android_Handle::log(std::string msg, std::string logTag)
	{
		std::stringstream str;
		str << "\n" << msg << "\n";
		std::string tmp = str.str();
		const char* cmsg = tmp.c_str();
		__android_log_print(ANDROID_LOG_DEBUG, logTag.c_str(), "%s", cmsg);
	}

	Android_File Android_Handle::open(std::string filename)
	{
		std::string folder = util::strip(filename, "/").at(0);
		if (folder == "assets") {
			AAsset* asset = AAssetManager_open(assetManager, filename.substr(7, std::string::npos).c_str(), AASSET_MODE_BUFFER);
			return Android_File(asset);
		}
		else {
			FILE* file = std::fopen(filename.c_str(), "r");
		}
	}
}