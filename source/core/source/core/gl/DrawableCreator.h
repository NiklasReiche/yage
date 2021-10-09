#pragma once

#include <memory>
#include <span>

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Drawable.h"

namespace gl
{
	enum class VertexFormat
	{
		INTERLEAVED,
		BATCHED
	};

	class IDrawableCreator
	{
	public:
		virtual ~IDrawableCreator() = default;

		/**
		 * @brief 
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
			const std::vector<unsigned int>& vertexLayout,
			VertexFormat format) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<IDrawable> createDrawable(
			const std::span<const unsigned char>& vertices,
			const std::span<const unsigned char>& indices,
			const std::vector<unsigned int>& vertexLayout,
			unsigned int nVertices,
			VertexFormat format) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ElementBuffer> createElementBuffer(const std::span<const unsigned int>& indices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<ElementBuffer> createElementBuffer(const std::span<const unsigned char>& indices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<VertexBuffer> createVertexBuffer(const std::span<const float>& vertices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<VertexBuffer> createVertexBuffer(const std::span<const unsigned char>& vertices) = 0;

		[[nodiscard]]
		virtual std::unique_ptr<VertexArray> createVertexArray(
			const std::shared_ptr<VertexBuffer>& vertexBuffer,
			const std::shared_ptr<ElementBuffer>& elementBuffer,
			const std::vector<unsigned int>& vertexLayout,
			gl::VertexFormat format = gl::VertexFormat::INTERLEAVED) = 0;

	protected:
		IDrawableCreator() = default;
		IDrawableCreator(const IDrawableCreator& other) = default;
		IDrawableCreator(IDrawableCreator&& other) = default;
		IDrawableCreator& operator=(const IDrawableCreator & other) = default;
		IDrawableCreator& operator=(IDrawableCreator&& other) = default;
	};
}
