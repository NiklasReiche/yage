#define GL3
#include <platform/Platform.h>
#include <input/InputController.h>
#include <graphics\Graphics.h>
#include <image/imageLoader.h>


int main()
{
	sys::PlatformHandle platform;
	gl::GraphicsContext glContext(&platform, 250, 250);
	input::InputController controller(&platform);


	img::ImageReader reader(&platform);
	img::Image image = reader.readFile("D:/DEV/projects/yage/tests/skyboxeditor/test.png");
	gl::Texture tex = glContext.create2DTexture(image.getRawData(), image.getWidth(), image.getHeight(), gl::ImageFormat::RGBA);

	tex.resize(200, 200);

	platform.getTimeStep();
	glContext.showWindow();
	while (!glContext.getCloseFlag())
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glContext.setActiveViewport(gl::Viewport(0, 0, tex.getWidth(), tex.getHeight()));
		glContext.draw(tex);

		glContext.swapBuffers();
		platform.pollEvents();
	}

	return 0;
}