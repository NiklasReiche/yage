#include "MousePosEvent.h"

namespace input
{
	MousePosEvent::MousePosEvent(const float xPos, const float yPos)
		: Event(Type::MOUSE_POS_EVENT), xPos(xPos), yPos(yPos) {}

	float MousePosEvent::getXPos() const
	{
		return xPos;
	}

	float MousePosEvent::getYPos() const
	{
		return yPos;
	}
}
