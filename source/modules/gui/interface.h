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
		sys::PlatformHandle * platform;
		gl::GraphicsContext* glContext;

		FontManager* fontManager;
		TextureManager* textureManager;
		InputManager* inputManger;
		GuiRenderer* renderer;
	};
}