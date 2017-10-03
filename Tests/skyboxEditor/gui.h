#include "gui/gui.h"


class Gui
{
private:
	sys::PlatformHandle* platform;
	gl::GraphicsContext* gl;
	gui::Master* master;

	gui::PushButton* button;
	gui::PushButton* button2;
	gui::PushButton* button3;
	gui::PushButton* button4;
	gui::PushButton* button5;
	gui::PushButton* button6;

	std::string lastPath = "D:/DEV/Projects/YAGE";

public:
	Gui(sys::PlatformHandle* platform, gl::GraphicsContext* gl, input::InputController* input);

	void update(double dt)
	{
		master->update(dt);
	}

	void on_button_click();
	void on_button_click2();
	void on_button_click3();
	void on_button_click4();
	void on_button_click5();
	void on_button_click6();
};