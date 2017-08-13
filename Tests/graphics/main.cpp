#define GL3
#include <platform/Platform.h>
#include <input/InputController.h>
#include <graphics\Graphics.h>

int main()
{
	sys::PlatformHandle platform;
	gl::GraphicsContext glContext(&platform, 250, 250);
	input::InputController controller(&platform);


	std::vector<gl::Gfloat> vertices1 = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};
	std::vector<gl::Gfloat> vertices2 = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,

		0.0f, 0.0f,
		0.0f, -1.0f,
		1.0f, -1.0f
	};
	std::vector<gl::Gfloat> vertices3 = {
		0.0f, 0.0f,
		0.0f, -1.0f,
		-1.0f, -1.0f
	};

	gl::Drawable drawable = glContext.createDrawable(vertices2, std::vector<int>{2}, gl::DrawMode::DRAW_STATIC);
	gl::Drawable drawable2 = glContext.createDrawable(vertices3, std::vector<int>{2}, gl::DrawMode::DRAW_STREAM);

	std::string vert =
#include "shader.vert"
		;
	std::string frag =
#include "shader.frag"
		;
	gl::Shader shader = glContext.compileShader(vert, frag);
	glContext.useShader(shader);

	gl::Framebuffer framebuffer = glContext.createFramebuffer(100, 100);
	gl::Viewport viewport = gl::Viewport(0, 0, 100, 100);

	
	double timer = 0.0;
	platform.getTimeStep();
	glContext.showWindow();
	while (!glContext.getCloseFlag())
	{
		glClear(GL_COLOR_BUFFER_BIT);
		controller.poll();
		if (controller.getInput()->getKey(input::KeyCode::KEY_W) == input::KeyState::DOWN && controller.getInput()->getKeyLast(input::KeyCode::KEY_W) == input::KeyState::UP) {
			drawable.bufferData(vertices1, 3);
			std::cout << "W" << std::endl;
		}
		if (controller.getInput()->getKey(input::KeyCode::KEY_Q) == input::KeyState::DOWN && controller.getInput()->getKeyLast(input::KeyCode::KEY_Q) == input::KeyState::UP) {
			drawable.bufferData(vertices2, 6);
			std::cout << "Q" << std::endl;
		}

		if (timer > 1) {
			//drawable.bufferData(vertices1, 3);
			timer = 0.0;
			std::cout << "timer" << std::endl;
		}


		glContext.draw(drawable);
		glContext.draw(drawable2);

		glContext.swapBuffers();

		timer += platform.getTimeStep();
	}

	return 0;
}