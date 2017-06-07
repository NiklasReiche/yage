#pragma once

#include "core.h"


namespace gui
{
	class FontManager;
	class InputManager;
	class GuiRenderer;

	struct ManagerInterface
	{
		FontManager* fontManager;
		InputManager* inputManger;
		GuiRenderer* renderer;
		gl::GraphicsContext* glContext;
	};
}