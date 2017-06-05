#include "input.h"

namespace gui
{
	InputManager::InputManager()
		: window(window)
	{
		keys.fill(InputAction::IDLE);
	}

	void InputManager::initialize(ngl::Window * window)
	{
		this->window = window;
	}

	void InputManager::update(Widget & root)
	{
		keys_pressed.resize(0);
		keys_released.resize(0);

		ngl::RawInput & input = window->getInputInfo();

		mouse_pos = input.mouse_pos;
		mouse_wheel_offset = input.mouse_wheel_offset;

		for (unsigned int i = 0; i < input.keys.size(); ++i)
		{
			if ((keys[i] == InputAction::RELEASE || keys[i] == InputAction::IDLE) && input.keys[i]) {
				keys[i] = InputAction::PRESS;
				keys_pressed.push_back(i);
			}
			else if (keys[i] == InputAction::PRESS && input.keys[i]) {
				keys[i] = InputAction::HOLD;
			}
			else if ((keys[i] == InputAction::PRESS || keys[i] == InputAction::HOLD) && !input.keys[i]) {
				keys[i] = InputAction::RELEASE;
				keys_released.push_back(i);
			}
			else if (keys[i] == InputAction::RELEASE && !input.keys[i]) {
				keys[i] = InputAction::IDLE;
			}
		}
		
		walkWidgets(root, 0);
	}

	bool InputManager::isHoveredOver(Widget & widget)
	{
		gml::Vec2<float> & w_pos = widget.getPosition();
		gml::Vec2<float> & w_size = widget.getSize();
		if (mouse_pos.x > w_pos.x && mouse_pos.x < (w_pos.x + w_size.x) && mouse_pos.y > w_pos.y && mouse_pos.y < (w_pos.y + w_size.y)) {
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
				if (child.isHovered && keys[GLFW_MOUSE_BUTTON_1] == InputAction::PRESS) {
					child.onClick();
				}
				else if (child.isHovered && keys[GLFW_MOUSE_BUTTON_1] == InputAction::RELEASE) {
					child.onClickRelease();
				}

				child.update();

				++level;
				walkWidgets(child, level);
			}
		}
	}
}