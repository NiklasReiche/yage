#pragma once

#include <span>
#include <vector>

namespace gl
{
	/**
	 * @brief Interface for objects that can be rendered.
	 */
	class IDrawable
	{
	public:
		virtual ~IDrawable() = default;

	protected:
		IDrawable() = default;
		IDrawable(const IDrawable& other) = default;
		IDrawable(IDrawable&& other) = default;
		IDrawable& operator=(const IDrawable& other) = default;
		IDrawable& operator=(IDrawable && other) = default;
	};
}
