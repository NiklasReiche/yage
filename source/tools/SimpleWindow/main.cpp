#include <platform/Window.h>
#include <platform/desktop/GlfwWindow.h>
#include <gl/Context.h>
#include <gl/opengl3/GL3_Context.h>

int main()
{
	std::shared_ptr<sys::IWindow> window = std::make_shared<sys::desktop::GlfwWindow>();

	std::shared_ptr<gl::IContext> context = std::make_shared<gl3::GL3_Context>(
		std::static_pointer_cast<sys::desktop::GlfwWindow>(window).get(), 500, 500);

	window->show();

	while (!window->shouldDestroy())
	{
		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
