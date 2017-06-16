#pragma once

#include <string>
#include <sstream>
#include <functional>
#include <string>
#include <vector>

#include <EGL/egl.h>
#include <android/log.h>
#include <android/input.h>

#include <platform/GenericHandle.h>

#include "android_native_app_glue.h"
#include "Android_File.h"

namespace android
{
	void handleCommand(struct android_app* app, int32_t cmd);
	int32_t handleInputEvent(struct android_app* app, AInputEvent* event);


	struct AppCallbacks
	{
		std::function<void()> onAppStart;
		std::function<void()> onAppResume;
		std::function<void()> onAppPause;
		std::function<void()> onAppStop;
		std::function<void()> onAppDestroy;

		std::function<void()> onWindowInit;
		std::function<void()> onWindowTerm;

		std::function<void(float, float, int, int)> onTouchEvent;
	};

	class Android_Handle : public platform::GenericPlatformHandle
	{
	private:
		android_app * app;
		EGLDisplay eglDisplay;
		EGLSurface eglSurface;
		EGLContext eglContext;

		AAssetManager* assetManager;

		void checkError(std::string msg);

	public:
		AppCallbacks callbacks;

		Android_Handle(android_app* app);
		~Android_Handle();

		void createContext(int * width, int * height, bool useGles3 = false);

		void makeCurrent();
		void pollEvents();
		void swapBuffers();

		int shouldDestroy();

		void log(std::string msg, std::string logTag = "LOG");
		void log(std::stringstream sstream);

		Android_File open(std::string filename);
		void read(std::string filename, std::stringstream & output);

		void setonAppStart(std::function<void()> callback) { callbacks.onAppStart = callback; }
		void setOnAppResume(std::function<void()> callback) { callbacks.onAppResume = callback; }
		void setonAppPause(std::function<void()> callback) { callbacks.onAppPause = callback; }
		void setonAppStop(std::function<void()> callback) { callbacks.onAppStop = callback; }
		void setonAppDestroy(std::function<void()> callback) { callbacks.onAppDestroy = callback; }
		void setOnWindowInit(std::function<void()> callback) { callbacks.onWindowInit = callback; }
		void setOnWindowTerm(std::function<void()> callback) { callbacks.onWindowTerm = callback; }
		void setOnTouchEvent(std::function<void(float, float, int, int)> callback) { callbacks.onTouchEvent = callback; }
	};
}