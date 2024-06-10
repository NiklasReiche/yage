#pragma once

#include <memory>
#include <span>

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Drawable.h"

namespace yage::gl
{
	enum class VertexFormat
	{
		INTERLEAVED,
		BATCHED
	};

    // TODO: fix the whole nVertices / nIndices ambiguity
	class IDrawableCreator
	{
	public:
		virtual ~IDrawableCreator() = default;

		/**
		 * Creates a drawable from float vertices and unsigned int indices.
		 * 
		 * @param vertices 
		 * @param vertexLayout 
		 * @param format 
		 *
		 * @return
		 *
		 * @throws std::invalid_argument The vertexLayout is empty or zero.
		 * @throws std::invalid_argument The format is not supported.
		 */
		[[nodiscard]]
		virtual std::unique_ptr<IDrawable> createDrawable(
                const std::span<const float> & vertices,
                const std::span<const unsigned int>& indices,
                const std::span<const unsigned int>& vertexLayout,
                gl::VertexFormat format) = 0;

        /**
         * Creates a drawable from a continuous data source. Vertices and indices are interpreted as byte streams,
         * where the the actual data type of the indices is computed by providing the total amount of vertices.
         * @param vertices byte data representing vertex data of type float
         * @param indices byte data representing indices of an unsigned integer type
         * @param vertex_layout layout of a vertex, i.e. a number for each component, representing the size of the component
         * @param n_indices the amount of indices in the given index buffer
         * @param format Whether the vertex data is interleaved or batched
         * @return a drawable
         */
		[[nodiscard]]
		virtual std::unique_ptr<IDrawable> createDrawable(
                const std::span<const std::byte>& vertices,
                const std::span<const std::byte>& indices,
                const std::span<const unsigned int>& vertex_layout,
                unsigned int n_indices,
                VertexFormat format) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ElementBuffer> createElementBuffer(const std::span<const unsigned int>& indices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ElementBuffer> createElementBuffer(const std::span<const std::byte>& indices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<VertexBuffer> createVertexBuffer(const std::span<const float>& vertices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<VertexBuffer> createVertexBuffer(const std::span<const std::byte>& vertices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<VertexArray> createVertexArray(
                const std::shared_ptr<gl::VertexBuffer>& vertex_buffer,
                const std::shared_ptr<gl::ElementBuffer>& element_buffer,
                const std::span<const unsigned int>& vertex_layout,
                unsigned int n_vertices,
                gl::VertexFormat format) = 0;

	protected:
		IDrawableCreator() = default;
		IDrawableCreator(const IDrawableCreator& other) = default;
		IDrawableCreator(IDrawableCreator&& other) = default;
		IDrawableCreator& operator=(const IDrawableCreator & other) = default;
		IDrawableCreator& operator=(IDrawableCreator&& other) = default;
	};
}
