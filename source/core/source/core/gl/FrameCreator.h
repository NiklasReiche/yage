#pragma once

#include <memory>

#include "Frame.h"
#include "TextureParams.h"

namespace yage::gl
{
	class IFrameCreator
	{
	public:
		virtual ~IFrameCreator() = default;
		
		virtual std::unique_ptr<IFrame> createFrame(
			int width,
			int height,
			ImageFormat format) = 0;

	protected:
		IFrameCreator() = default;
		IFrameCreator(const IFrameCreator& other) = default;
		IFrameCreator(IFrameCreator&& other) = default;
		IFrameCreator& operator=(const IFrameCreator& other) = default;
		IFrameCreator& operator=(IFrameCreator&& other) = default;
	};
}
