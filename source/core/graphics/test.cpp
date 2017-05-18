#define GL3
#include "Graphics.h"
#include <iostream>

int main()
{
	gl::Gchar var = 5;

	gl::GraphicsContext context(500, 500);
	context.showWindow();

	std::cin.get();
	return 0;
}