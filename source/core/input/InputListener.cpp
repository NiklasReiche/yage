#include "InputListener.h"
#include "InputController.h"

namespace input
{
	InputListener::InputListener(int id)
		: id(id) {}

	InputState* InputListener::getInput()
	{
		return controller->getInput();
	}
}