#include "KeyEvent.h"

namespace input 
{
	KeyEvent::KeyEvent(const Code key, const Action action)
		: Event(Type::KEY_EVENT), key(key), action(action) {}

	KeyEvent::Code KeyEvent::getKey() const
	{
		return key;
	}

	KeyEvent::Action KeyEvent::getAction() const
	{
		return action;
	}
}
