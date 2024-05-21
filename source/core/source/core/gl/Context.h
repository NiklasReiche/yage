#pragma once

#include <memory>

#include "Renderer.h"
#include "TextureCreator.h"
#include "ShaderCreator.h"
#include "DrawableCreator.h"
#include "FrameCreator.h"

namespace yage::platform
{
	class IWindow;
}

namespace yage::gl
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

	std::shared_ptr<IContext> createOpenGlContext(const std::weak_ptr<platform::IWindow>& window);

    std::shared_ptr<IContext> createVulkanContext(const std::weak_ptr<platform::IWindow>& window);
}
