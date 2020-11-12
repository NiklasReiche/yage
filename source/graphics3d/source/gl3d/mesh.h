#pragma once

#include <memory>

#include <core/gl/Drawable.h>

namespace gl3d
{
	class Mesh
	{
	public:
		std::unique_ptr<gl::IDrawable> drawable;
	};
}