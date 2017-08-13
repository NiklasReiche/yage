#pragma once

#include "core.h"

namespace gui
{
	class FontManager;
	class InputManager;
	class GuiRenderer;

	struct MasterInterface
	{
		sys::PlatformHandle * platform;
		gl::GraphicsContext* glContext;

		FontManager* fontManager;
		InputManager* inputManger;
		GuiRenderer* renderer;
	};
}