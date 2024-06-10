#pragma once

#include "Event.h"

namespace yage::input
{
	class MouseWheelEvent final : public Event
	{
	public:
		MouseWheelEvent(float xOffset, float yOffset);

		~MouseWheelEvent() = default;

		[[nodiscard]]
		float getXOffset() const;
		[[nodiscard]]
		float getYOffset() const;

	private:
		const float xOffset;
		const float yOffset;
	};
}
