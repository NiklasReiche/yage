#pragma once

#include "core.h"
#include "widget.h"


namespace gui
{
	enum class InputAction
	{
		IDLE,
		HOLD,
		PRESS,
		RELEASE
	};

	class InputManager
	{
	private:
		ngl::Window * window;

		std::array<InputAction, 512> keys;
		std::vector<int> keys_pressed;
		std::vector<int> keys_released;

		gml::Vec2<float> mouse_pos;
		gml::Vec2<float> mouse_wheel_offset;

		bool isHoveredOver(Widget & widget);
	public:
		InputManager();
		void initialize(ngl::Window * window);

		void update(Widget & root);
		void walkWidgets(Widget & widget, int level);

		gml::Vec2<float> getMousePos() { return mouse_pos; }
		InputAction getKeyState(int key) { return keys[key]; }
		std::vector<int>& getPressedKeys() { return keys_pressed; }
		std::vector<int>& getReleasedKeys() { return keys_released; }
	};
}