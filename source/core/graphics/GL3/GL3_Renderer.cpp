// ReSharper disable CppClangTidyCppcoreguidelinesProTypeStaticCastDowncast
#include "GL3_Renderer.h"
#include "GL3_Context.h"
#include "GL3_Drawable.h"
#include "GL3_Texture2D.h"
#include "GL3_Texture3D.h"
#include "GL3_Cubemap.h"
#include "GL3_Shader.h"
#include "GL3_Framebuffer.h"
#include "GL3_UnitShader.h"
#include "../color.h"

namespace gl3
{
	std::unique_ptr<gl::IDrawable> GL3_Renderer::unitDrawable;
	std::unique_ptr<gl::IShader> GL3_Renderer::unitShader;
	
	void GL3_Renderer::enableDepthTest()
	{
		lockContextPtr()->enableDepthTest(GL_LESS, GL_TRUE);
	}

	void GL3_Renderer::disableDepthTest()
	{
		lockContextPtr()->disableDepthTest();
	}

	void GL3_Renderer::setDepthTest(const bool value)
	{
		if (value) 
			enableDepthTest();
		else 
			disableDepthTest();
	}

	void GL3_Renderer::enableStencilTest()
	{
		lockContextPtr()->enableStencilTest();
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	void GL3_Renderer::disableStencilTest()
	{
		lockContextPtr()->disableStencilTest();
	}

	void GL3_Renderer::setStencilTest(const bool value)
	{
		if (value)
			enableStencilTest();
		else
			disableStencilTest();
	}

	void GL3_Renderer::enableBlending()
	{
		lockContextPtr()->enableBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void GL3_Renderer::disableBlending()
	{
		lockContextPtr()->disableBlending();
	}

	void GL3_Renderer::setBlending(const bool value)
	{
		if (value)
			enableBlending();
		else
			disableBlending();
	}

	void GL3_Renderer::enableWireframe()
	{
		lockContextPtr()->setPolygonMode(GL_LINE);
	}

	void GL3_Renderer::disableWireframe()
	{
		lockContextPtr()->setPolygonMode(GL_FILL);
	}

	void GL3_Renderer::setWireframe(const bool value)
	{
		if (value)
			enableWireframe();
		else
			disableWireframe();
	}

	void GL3_Renderer::setViewport(const int x, const int y, const int width, const int height)
	{
		setViewport({x, y, width, height});
	}
	
	void GL3_Renderer::setViewport(const Viewport viewport)
	{
		auto ptr = lockContextPtr();
		ptr->setViewport(viewport.x, ptr->getRenderTargetHeight() - (viewport.y + viewport.height), viewport.width, viewport.height);
	}

	void GL3_Renderer::clear()
	{
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void GL3_Renderer::setClearColor(const uint32_t color)
	{
		clearColor = gl::toVec4(color);
	}

	void GL3_Renderer::setRenderTarget(const gl::IFrame & target)
	{
		lockContextPtr()->bindFramebuffer(
			GL_FRAMEBUFFER, 
			static_cast<const GL3_Framebuffer&>(target).FBO);
	}

	void GL3_Renderer::draw(const gl::IDrawable & drawable)
	{
		auto& ptr = static_cast<const GL3_Drawable&>(drawable);
		lockContextPtr()->bindVertexArray(ptr.VAO);
		glDrawArrays(static_cast<GLenum>(ptr.primitive), 0, ptr.nVertices);
	}

	void GL3_Renderer::draw(const gl::IFrame & buffer)
	{
		lockContextPtr()->bindFramebuffer(GL_FRAMEBUFFER, 0);
		useShader(*unitShader);
		bindTexture(*static_cast<const GL3_Framebuffer&>(buffer).texture);
		draw(*unitDrawable);
	}

	void GL3_Renderer::draw(const gl::ITexture2D & texture)
	{
		useShader(*unitShader);
		bindTexture(texture);
		draw(*unitDrawable);
	}

	void GL3_Renderer::useShader(const gl::IShader & shader)
	{
		lockContextPtr()->bindShader(static_cast<const GL3_Shader&>(shader).getId());
	}

	void GL3_Renderer::bindTexture(const gl::ITexture2D & texture, const int unit)
	{
		const auto& ptr = static_cast<const GL3_Texture2D&>(texture);
		bindTexture(static_cast<const GL3_Texture&>(ptr), unit);
	}

	void GL3_Renderer::bindTexture(const gl::ITexture3D& texture, const int unit)
	{
		const auto& ptr = static_cast<const GL3_Texture3D&>(texture);
		bindTexture(static_cast<const GL3_Texture&>(ptr), unit);
	}

	void GL3_Renderer::bindTexture(const gl::ICubemap& texture, const int unit)
	{
		const auto& ptr = static_cast<const GL3_Cubemap&>(texture);
		bindTexture(static_cast<const GL3_Texture&>(ptr), unit);
	}

	GL3_Renderer::GL3_Renderer(std::weak_ptr<GL3_Context> contextPtr)
		: GL3_BaseObject(std::move(contextPtr))
	{
		if (!unitDrawable)
		{
			unitDrawable = lockContextPtr()->getDrawableCreator()->createDrawable(
				GL3_UnitShaderTemplate::vertices,
				GL3_UnitShaderTemplate::vertexLayout,
				gl::VertexFormat::INTERLEAVED);
			unitShader = lockContextPtr()->getShaderCreator()->createShader(
				GL3_UnitShaderTemplate::vertexCode,
				GL3_UnitShaderTemplate::fragmentCode);
			unitShader->setUniform("screenTexture", 0);
		}
	}

	void GL3_Renderer::bindTexture(const GL3_Texture& texture, const int unit)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(texture.target), texture.id, unit);
	}
}
