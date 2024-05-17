#pragma once

#include "../DrawableCreator.h"

#include "BaseObject.h"
#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace opengl
{
	class DrawableCreator final : public BaseObject, public gl::IDrawableCreator
	{
	public:
		std::unique_ptr<gl::IDrawable> createDrawable(
			const std::span<const float>& vertices,
			const std::span<const unsigned int>& indices,
			const std::vector<unsigned int>& vertexLayout,
			gl::VertexFormat format = gl::VertexFormat::INTERLEAVED) override;

		std::unique_ptr<gl::IDrawable> createDrawable(
			const std::span<const unsigned char>& vertices,
			const std::span<const unsigned char>& indices,
			const std::vector<unsigned int>& vertexLayout,
			unsigned int nIndices,
			gl::VertexFormat format = gl::VertexFormat::INTERLEAVED) override;

		std::unique_ptr<gl::ElementBuffer> createElementBuffer(const std::span<const unsigned int>& indices) override;

		std::unique_ptr<gl::ElementBuffer> createElementBuffer(const std::span<const unsigned char>& indices) override;

		std::unique_ptr<gl::VertexBuffer> createVertexBuffer(const std::span<const float>& vertices) override;

		std::unique_ptr<gl::VertexBuffer> createVertexBuffer(const std::span<const unsigned char>& vertices) override;

		std::unique_ptr<gl::VertexArray> createVertexArray(
			const std::shared_ptr<gl::VertexBuffer>& vertexBuffer,
			const std::shared_ptr<gl::ElementBuffer>& elementBuffer,
			const std::vector<unsigned int>& vertexLayout,
            unsigned int nIndices,
			gl::VertexFormat format) override;

	private:
		using BaseObject::BaseObject;

		std::unique_ptr<VertexArray> createVertexArrayInternal(
			const std::shared_ptr<gl::VertexBuffer>& vertexBuffer,
			const std::shared_ptr<gl::ElementBuffer>& elementBuffer,
			const std::vector<unsigned int>& vertexLayout,
            unsigned int nVertices,
			gl::VertexFormat format);

		std::unique_ptr<gl::ElementBuffer> createElementBuffer(const unsigned char* indices, std::size_t size);

		std::unique_ptr<gl::VertexBuffer> createVertexBuffer(const unsigned char* vertices, std::size_t size);

		friend class Context;
	};
}// namespace gl3
