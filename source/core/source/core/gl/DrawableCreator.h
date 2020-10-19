#pragma once

#include <memory>
#include <span>

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
			const std::vector<unsigned int>& vertexLayout,
			VertexFormat format) = 0;

	protected:
		IDrawableCreator() = default;
		IDrawableCreator(const IDrawableCreator& other) = default;
		IDrawableCreator(IDrawableCreator&& other) = default;
		IDrawableCreator& operator=(const IDrawableCreator & other) = default;
		IDrawableCreator& operator=(IDrawableCreator&& other) = default;
	};
}
