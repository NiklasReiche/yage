#pragma once

#include <vector>

#include "../Drawable.h"
#include "../DrawableCreator.h"
#include "GlEnum.h"
#include "OpenGlObject.h"
#include "OpenGL.h"
#include "VertexArray.h"

namespace opengl
{
	class Drawable final : public gl::IDrawable, std::enable_shared_from_this<Drawable>
	{
	public:
		Drawable() = default;
		~Drawable() override = default;
		Drawable(const Drawable& other) = delete;
		Drawable& operator=(const Drawable& other) = delete;

	private:
		std::unique_ptr<VertexArray> vertexArray;
		GLint nVertices = 0;
		GLenum indicesDataType = GL_UNSIGNED_INT;

		Drawable(Drawable&& other) noexcept;
		Drawable& operator=(Drawable&& other) noexcept;

		friend class Context;
		friend class Renderer;
		friend class DrawableCreator;
	};
}
