#pragma once

#include <span>
#include <vector>

namespace yage::gl
{
	/**
	 * @brief Interface for objects that can be rendered.
	 */
	class IDrawable
	{
	public:
		virtual ~IDrawable() = default;

        virtual void setSubData(unsigned int offset, const std::vector<float>& vertices) = 0;

	protected:
		IDrawable() = default;
		IDrawable(const IDrawable& other) = default;
		IDrawable(IDrawable&& other) = default;
		IDrawable& operator=(const IDrawable& other) = default;
		IDrawable& operator=(IDrawable && other) = default;
	};
}
