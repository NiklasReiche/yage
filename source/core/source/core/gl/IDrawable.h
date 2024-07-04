#pragma once

#include "enums.h"

namespace yage::gl
{
	/**
	 * Interface for objects that can be rendered.
	 */
	class IDrawable2
	{
	public:
		virtual ~IDrawable2() = default;

	    [[nodiscard]] virtual VertexDataInfo vertex_data_info() const = 0;
	};
}
