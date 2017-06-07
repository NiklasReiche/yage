#include "input.h"

namespace gui
{
	void InputManager::initialize(input::InputController * inputController)
	{
		this->inputController = inputController;
	}

	void InputManager::update(Widget & root)
	{
		input = inputController->getInput();
		
		walkWidgets(root, 0);
	}

	bool InputManager::isHoveredOver(Widget & widget)
	{
		gml::Vec2<float> & w_pos = widget.getPosition();
		gml::Vec2<float> & w_size = widget.getSize();
		if (input->getMousePos().x > w_pos.x && input->getMousePos().x < (w_pos.x + w_size.x) && input->getMousePos().y > w_pos.y && input->getMousePos().y < (w_pos.y + w_size.y)) {
			return true;
		}
		return false;
	}

	void InputManager::walkWidgets(Widget & widget, int level)
	{
		for (unsigned int i = 0; i < widget.getChildrenCount(); ++i)
		{
			Widget & child = widget.getChild(i);

			if (child.is_Active()) {
				if (isHoveredOver(child) && !child.isHoverActive()) {
					child.isHovered = true;
					child.onHover();
				}
				else if (!isHoveredOver(child) && child.isHoverActive()) {
					child.isHovered = false;
					child.onHoverRelease();
				}
				if (child.isHovered && input->getMouseKey(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::PRESS) {
					child.onClick();
				}
				else if (child.isHovered &&  input->getMouseKey(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::RELEASE) {
					child.onClickRelease();
				}

				child.update();

				++level;
				walkWidgets(child, level);
			}
		}
	}
}