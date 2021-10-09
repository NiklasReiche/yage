#pragma once

#include <span>
#include <vector>
#include "../VertexBuffer.h"
#include "OpenGL.h"
#include "OpenGlObject.h"
#include "GlEnum.h"

namespace opengl
{
	class VertexBuffer : public gl::VertexBuffer, public OpenGlObject, public std::enable_shared_from_this<VertexBuffer>
	{
	public:
		~VertexBuffer() override;

		VertexBuffer(const VertexBuffer& other) = delete;

		VertexBuffer& operator=(const VertexBuffer& other) = delete;

		void setData(const std::span<const float>& vertices) override;

		void setSubData(unsigned int offset, const std::span<const float>& vertices) override;

		std::vector<float> getSubData(unsigned int offset, unsigned int size) override;

		bool isEmpty() override;

	private:
		GLuint vbo = 0;
		size_t byteSize = 0;
		DrawMode usage = DrawMode::DRAW_STATIC;
		bool empty = true;

		using OpenGlObject::OpenGlObject;

		VertexBuffer(VertexBuffer&& other) noexcept;

		VertexBuffer& operator=(VertexBuffer&& other) noexcept;

		friend class Context;

		friend class Renderer;

		friend class DrawableCreator;
	};
}
