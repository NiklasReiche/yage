#pragma once

#include <memory>

#include <core/gl/Drawable.h>

namespace gl3d
{
	class SceneRenderer;

	class Mesh
	{
	public:
		explicit Mesh(std::shared_ptr<gl::IDrawable> drawable);

	private:
		std::shared_ptr<gl::IDrawable> drawable;

		friend class SceneRenderer;
	};
}