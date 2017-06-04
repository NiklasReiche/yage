#include "AndroidApp.h"

namespace android
{
	void handleCommand(struct android_app* app, int32_t cmd)
	{
		AndroidApp* androidApp = (AndroidApp*)app->userData;
		switch (cmd)
		{
		case APP_CMD_INPUT_CHANGED:
			break;

		case APP_CMD_INIT_WINDOW:
			androidApp->onWindowInit();
			break;

		case APP_CMD_TERM_WINDOW:
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
			androidApp->onAppStart();
			break;

		case APP_CMD_RESUME:
			androidApp->onAppResume();
			break;

		case APP_CMD_SAVE_STATE:
			break;

		case APP_CMD_PAUSE:
			androidApp->onAppPause();
			break;

		case APP_CMD_STOP:
			androidApp->onAppStop();
			break;

		case APP_CMD_DESTROY:
			androidApp->onAppDestroy();
			break;
		}
	}
	int32_t handleInputEvent(struct android_app* app, AInputEvent* event)
	{

	}

	AndroidApp::AndroidApp(android_app* app)
		: androidHandle(app)
	{
		app_dummy();
		androidHandle->userData = this;
		androidHandle->onAppCmd = handleCommand;
		androidHandle->onInputEvent = handleInputEvent;
	}
	AndroidApp::~AndroidApp()
	{

	}

	bool AndroidApp::isInitialized()
	{
		if (androidHandle->window == NULL) {
			return false;
		}
		else {
			return true;
		}
	}

	void AndroidApp::log(std::string msg, std::string logTag)
	{
		std::stringstream str;
		str << "\n" << msg << "\n";
		std::string tmp = str.str();
		const char* cmsg = tmp.c_str();
		__android_log_print(ANDROID_LOG_DEBUG, logTag.c_str(), cmsg);
	}
	void AndroidApp::pollEvents(int timeout)
	{
		int ident;
		int events;
		android_poll_source* source;
		
		ident = ALooper_pollAll(timeout, NULL, &events, (void**)&source);

		if (source != NULL) {
			source->process(androidHandle, source);
		}
	}

	void AndroidApp::onWindowInit()
	{
		if (androidHandle->window != NULL) {
			try {
				callbacks.onWindowInit();
			}
			catch (std::bad_function_call) {

			}
		}
	}
}