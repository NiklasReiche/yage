#pragma once

#include "core.h"

namespace gui
{
	class FontManager;
	class InputManager;
	class GuiRenderer;
	class TextureManager;

	struct MasterInterface
	{
		sys::Window platform;
		gl::Context glContext;

		FontManager* fontManager;
		TextureManager* textureManager;
		InputManager* inputManger;
		GuiRenderer* renderer;
	};
}