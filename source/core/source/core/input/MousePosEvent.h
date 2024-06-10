#pragma once

#include "Event.h"

namespace yage::input
{
	class MousePosEvent final : public Event
	{
	public:
		MousePosEvent(float xPos, float yPos);

		~MousePosEvent() = default;

		[[nodiscard]]
		float getXPos() const;
		[[nodiscard]]
		float getYPos() const;

	private:
		const float xPos;
		const float yPos;
	};
}
