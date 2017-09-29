#include "gui/gui.h"

class Gui
{
private:
	sys::PlatformHandle* platform;
	gl::GraphicsContext* gl;
	gui::Master* master;

public:
	Gui(sys::PlatformHandle* platform, gl::GraphicsContext* gl, input::InputController* input);

	void update(double dt)
	{
		master->update(dt);
	}
};