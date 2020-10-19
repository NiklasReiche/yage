#pragma once

#include <vector>

#include "../Drawable.h"
#include "../DrawableCreator.h"
#include "GlEnum.h"
#include "OpenGlObject.h"
#include "OpenGL.h"

namespace opengl
{
	class Drawable final : public OpenGlObject, public gl::IDrawable, std::enable_shared_from_this<Drawable>
	{
	public:
		~Drawable() override;
		Drawable(const Drawable& other) = delete;
		Drawable& operator=(const Drawable& other) = delete;

		void setData(const std::span<const float>& vertices) override;
		void setSubData(unsigned int offset, const std::span<const float>& vertices) override;
		std::vector<float> getSubData(unsigned int offset, unsigned int size) override;
		bool isEmpty() override;

	private:
		GLuint VAO = 0;
		GLuint VBO = 0;
		PrimitiveType primitive = PrimitiveType::TRIANGLES;
		DrawMode usage = DrawMode::DRAW_STATIC;
		GLint nVertices = 0;
		std::vector<unsigned int> layout;
		unsigned int vertexSize = 0;
		gl::VertexFormat format = gl::VertexFormat::BATCHED;
		size_t bufferSize = 0;
		bool empty = true;

		using OpenGlObject::OpenGlObject;

		Drawable(Drawable&& other) noexcept;
		Drawable& operator=(Drawable&& other) noexcept;

		friend class Context;
		friend class Renderer;
		friend class DrawableCreator;
	};
}
