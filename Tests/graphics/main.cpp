#define GL3
#include <platform/Platform.h>
#include <graphics\Graphics.h>

int main()
{
	platform::PlatformHandle platform;
	gl::GraphicsContext glContext(&platform, 250, 250);


	std::vector<gl::Gfloat> vertices = {
		0.0f, 0.0f,
		0.0f, 0.5f,
		0.5f, 0.5f
	};
	gl::Drawable drawable = glContext.createDrawable(vertices, std::vector<int>{2}, gl::DrawMode::DRAW_STATIC);

	gl::ShaderLoader shaderLoader(&glContext);
	gl::Shader shader = shaderLoader.loadShader("shaders/shader.vert", "shaders/shader.frag");

	gl::Framebuffer framebuffer = glContext.createFramebuffer(100, 100);
	gl::Viewport viewport = gl::Viewport(0, 0, 100, 100);

	
	glContext.showWindow();
	while (!glContext.getCloseFlag())
	{
		glContext.swapBuffers();
	}

	return 0;
}