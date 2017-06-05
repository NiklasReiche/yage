#define GL3
#include <graphics\Graphics.h>

int main()
{
	gl::GraphicsContext glContext(250, 250);
	glContext.showWindow();

	while (!glContext.getCloseFlag())
	{
		glContext.swapBuffers();
	}

	return 0;
}