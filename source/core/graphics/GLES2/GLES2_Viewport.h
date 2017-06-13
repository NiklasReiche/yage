#pragma once

namespace gles2
{
	struct GLES2_Viewport
	{
		int x, y;	// Top Left Corner
		int width, height;

		GLES2_Viewport() {}
		GLES2_Viewport(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height) {}
	};
}