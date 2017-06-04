#pragma once

#include <string>
#include <sstream>
#include <functional>

#include <android/log.h>
#include "android_native_app_glue.h"

namespace android
{
	enum class AppState
	{
		RUNNING,
		PAUSED,
		STOPPED
	};

	struct AppCallbacks
	{
		std::function<void()> onWindowInit;
		std::function<void()> onWindowTerm;
	};

	void handleCommand(struct android_app* app, int32_t cmd);
	int32_t handleInputEvent(struct android_app* app, AInputEvent* event);

	class AndroidApp
	{
	private:
		android_app * androidHandle;
		AppState state;
		AppCallbacks callbacks;

	public:
		AndroidApp(android_app* app);
		~AndroidApp();

		bool isInitialized();
		void log(std::string msg, std::string logTag = "LOG");
		void pollEvents(int timeout);


		void onAppStart() {};
		void onAppResume() {};
		void onAppPause() {};
		void onAppStop() {};
		void onAppDestroy() {};

		void onWindowInit();
		void onWindowTerm() {};

		void setOnWindowInit(std::function<void()> callback) { callbacks.onWindowInit = callback; }
		void setOnWindowTerm(std::function<void()> callback) { callbacks.onWindowTerm = callback; }

		ANativeWindow* getWindow() { return androidHandle->window; }
	};
}