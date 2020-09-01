#pragma once

#include <memory>

#include <platform/>

#include "Renderer.h"
#include "TextureCreator.h"
#include "ShaderCreator.h"
#include "DrawableCreator.h"
#include "FrameCreator.h"

namespace gl
{
	class IContext
	{
	public:
		virtual ~IContext() = default;
		
		virtual std::shared_ptr<IDrawableCreator> getDrawableCreator() = 0;
		virtual std::shared_ptr<ITextureCreator> getTextureCreator() = 0;
		virtual std::shared_ptr<IFrameCreator> getFrameCreator() = 0;
		virtual std::shared_ptr<IShaderCreator> getShaderCreator() = 0;
		virtual std::shared_ptr<IRenderer> getRenderer() = 0;

	protected:
		IContext() = default;
		IContext(const IContext& other) = default;
		IContext(IContext&& other) = default;
		IContext& operator=(const IContext & other) = default;
		IContext& operator=(IContext&& other) = default;
	};

	std::shared_ptr<IContext> createContext();
}
