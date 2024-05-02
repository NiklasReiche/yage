#pragma once

#include "../DrawableCreator.h"
#include "../VertexArray.h"
#include "OpenGlObject.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

namespace opengl
{
	class VertexArray : public gl::VertexArray, public OpenGlObject
	{
	public:
		~VertexArray() override;

		VertexArray(VertexArray& other) = delete;

		VertexArray& operator=(const VertexArray& other) = delete;

        void setSubData(unsigned int offset, const std::vector<float>& vertices);

	private:
		GLuint vao = 0;
		std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
		std::shared_ptr<ElementBuffer> elementBuffer = nullptr;
		PrimitiveType primitive = PrimitiveType::TRIANGLES;
		std::vector<unsigned int> layout;
		unsigned int vertexSize = 0;
		gl::VertexFormat format = gl::VertexFormat::BATCHED;

		using OpenGlObject::OpenGlObject;

		VertexArray(VertexArray&& other) noexcept;

		VertexArray& operator=(VertexArray&& other) noexcept;

		friend class Context;

		friend class Renderer;

		friend class DrawableCreator;
	};
}
