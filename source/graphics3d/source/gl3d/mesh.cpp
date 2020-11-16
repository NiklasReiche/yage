#include "mesh.h"

namespace gl3d
{
	Mesh::Mesh(std::shared_ptr<gl::IDrawable> drawable)
		: drawable(std::move(drawable))
	{ }
}
