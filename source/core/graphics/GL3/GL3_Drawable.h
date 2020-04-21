#pragma once

#include <vector>

#include <platform/OpenGL.h>
#include "../Drawable.h"
#include "../DrawableCreator.h"
#include "GL3_Object.h"
#include "GL3_Enum.h"

namespace gl3
{
	class GL3_Drawable final : public GL3_Object, public gl::IDrawable, std::enable_shared_from_this<GL3_Drawable>
	{
	public:
		virtual ~GL3_Drawable();
		GL3_Drawable(const GL3_Drawable& other) = delete;
		GL3_Drawable& operator=(const GL3_Drawable& other) = delete;

		void setData(const std::vector<GLfloat>& vertices) override;
		void setSubData(unsigned int offset, const std::vector<GLfloat>& vertices) override;
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

		using GL3_Object::GL3_Object;
		
		GL3_Drawable(GL3_Drawable&& other) noexcept;
		GL3_Drawable& operator=(GL3_Drawable&& other) noexcept;

		friend class GL3_Context;
		friend class GL3_Renderer;
		friend class GL3_DrawableCreator;
	};
}
