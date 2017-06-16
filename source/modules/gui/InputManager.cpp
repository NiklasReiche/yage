#include "InputManager.h"

namespace gui
{
	InputManager::InputManager(input::InputListener * inputListener)
		: inputListener(inputListener) {}

	void InputManager::update(Widget & root)
	{
		input = inputListener->getInput();
		
		walkWidgets(root, 0);
	}

	bool InputManager::isHoveredOver(Widget & widget)
	{
		gml::Vec2<float> w_pos = widget.getPosition();
		gml::Vec2<float> w_size = widget.getSize();
#ifdef DESKTOP
		if (input->getMousePos().x > w_pos.x && input->getMousePos().x < (w_pos.x + w_size.x) && input->getMousePos().y > w_pos.y && input->getMousePos().y < (w_pos.y + w_size.y)) {
			return true;
		}
#endif
#ifdef ANDROID
		if (input->getTouchPos(input::TouchIndexCode::TOUCH_INDEX_1).x > w_pos.x && input->getTouchPos(input::TouchIndexCode::TOUCH_INDEX_1).x < (w_pos.x + w_size.x) && input->getTouchPos(input::TouchIndexCode::TOUCH_INDEX_1).y > w_pos.y && input->getTouchPos(input::TouchIndexCode::TOUCH_INDEX_1).y < (w_pos.y + w_size.y)) {
			return true;
		}
#endif
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
#ifdef DESKTOP
				if (child.isHovered && input->getMouseKey(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::DOWN && input->getMouseKeyLast(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::UP) {
					child.onClick();
				}
				else if (child.isHovered && input->getMouseKey(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::UP && input->getMouseKeyLast(input::MouseKeyCode::KEY_MOUSE_1) == input::KeyState::DOWN) {
					child.onClickRelease();
				}
#endif
#ifdef ANDROID
				if (child.isHovered && input->getTouchState(input::TouchIndexCode::TOUCH_INDEX_1) == input::KeyState::DOWN && input->getTouchStateLast(input::TouchIndexCode::TOUCH_INDEX_1) == input::KeyState::UP) {
					child.onClick();
				}
				else if (child.isHovered && input->getTouchState(input::TouchIndexCode::TOUCH_INDEX_1) == input::KeyState::UP && input->getTouchStateLast(input::TouchIndexCode::TOUCH_INDEX_1) == input::KeyState::DOWN) {
					child.onClickRelease();
				}
#endif

				child.update();

				++level;
				walkWidgets(child, level);
			}
		}
	}

	void InputManager::onMouseEvent(input::MouseKeyCode, input::KeyAction)
	{
		
	}
}