#include "MouseWheelEvent.h"

namespace yage::input
{
	MouseWheelEvent::MouseWheelEvent(const float xOffset, const float yOffset)
		: Event(Type::MOUSE_WHEEL_EVENT), xOffset(xOffset), yOffset(yOffset) {}

	float MouseWheelEvent::getXOffset() const
	{
		return xOffset;
	}

	float MouseWheelEvent::getYOffset() const
	{
		return yOffset;
	}

}
