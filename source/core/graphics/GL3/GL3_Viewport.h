#pragma once

namespace gl3
{
	struct Viewport
	{
		int x, y;	// Top Left Corner
		int width, height;

		Viewport() {}
		Viewport(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
	};
}