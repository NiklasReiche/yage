#pragma once

#include "core.h"

namespace gui
{
	class FontManager;
	class InputManager;
	class GuiRenderer;
	class TextureManager;

    // TODO: we can probably do away with this - it is quite ugly
	struct MasterInterface
	{
		std::shared_ptr<platform::IWindow> platform;
		std::shared_ptr<gl::IContext> glContext;

		TextureManager* textureManager;
		InputManager* inputManger;
		GuiRenderer* renderer;
	};
}