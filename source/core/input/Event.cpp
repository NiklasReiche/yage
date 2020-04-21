#include "Event.h"

namespace input
{
	Event::Event(const Type type) noexcept
		: type(type) {}

	Event::Type Event::getType() const noexcept
	{
		return type;
	}
}
