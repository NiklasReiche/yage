#pragma once

#include "Handle.h"

namespace yage::gl
{
	/**
	 * Interface for two-dimensional textures.
	 */
	class ITexture2D2
	{
	public:
		virtual ~ITexture2D2() = default;
	};

    using Texture2DHandle = Handle<ITexture2D2>;
}
