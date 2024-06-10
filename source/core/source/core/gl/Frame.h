#pragma once

#include <memory>

#include "Texture2D.h"

namespace yage::gl
{
	class IFrame
	{
	public:
		virtual ~IFrame() = default;

		[[nodiscard]]
		virtual std::shared_ptr<ITexture2D> getTexture() const = 0;
		[[nodiscard]]
		virtual int getWidth() const = 0;
		[[nodiscard]]
		virtual int getHeight() const = 0;
		[[nodiscard]]
		virtual int getChannels() const = 0;

	protected:
		IFrame() = default;
		IFrame(const IFrame& other) = default;
		IFrame(IFrame&& other) = default;
		IFrame& operator=(const IFrame & other) = default;
		IFrame& operator=(IFrame&& other) = default;
	};
}
