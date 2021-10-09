// ReSharper disable CppClangTidyCppcoreguidelinesProTypeStaticCastDowncast
#include <iostream>
#include "Renderer.h"
#include "../color.h"
#include "Context.h"
#include "Cubemap.h"
#include "Drawable.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Texture2D.h"
#include "UnitShader.h"

namespace opengl
{
	std::unique_ptr<gl::IDrawable> Renderer::unitDrawable;
	std::unique_ptr<gl::IShader> Renderer::unitShader;

	void Renderer::enableDepthTest()
	{
		lockContextPtr()->enableDepthTest(GL_LESS, GL_TRUE);
	}

	void Renderer::disableDepthTest()
	{
		lockContextPtr()->disableDepthTest();
	}

	void Renderer::setDepthTest(const bool value)
	{
		if (value)
			enableDepthTest();
		else
			disableDepthTest();
	}

	void Renderer::enableStencilTest()
	{
		lockContextPtr()->enableStencilTest();
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	void Renderer::disableStencilTest()
	{
		lockContextPtr()->disableStencilTest();
	}

	void Renderer::setStencilTest(const bool value)
	{
		if (value)
			enableStencilTest();
		else
			disableStencilTest();
	}

	void Renderer::enableBlending()
	{
		lockContextPtr()->enableBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::disableBlending()
	{
		lockContextPtr()->disableBlending();
	}

	void Renderer::setBlending(const bool value)
	{
		if (value)
			enableBlending();
		else
			disableBlending();
	}

	void Renderer::enableWireframe()
	{
		lockContextPtr()->setPolygonMode(GL_LINE);
	}

	void Renderer::disableWireframe()
	{
		lockContextPtr()->setPolygonMode(GL_FILL);
	}

	void Renderer::setWireframe(const bool value)
	{
		if (value)
			enableWireframe();
		else
			disableWireframe();
	}

	void Renderer::setViewport(const int x, const int y, const int width, const int height)
	{
		setViewport({ x, y, width, height });
	}

	void Renderer::setViewport(const Viewport viewport)
	{
		auto ptr = lockContextPtr();
		ptr->setViewport(viewport.x, ptr->getRenderTargetHeight() - (viewport.y + viewport.height), viewport.width,
		                 viewport.height);
	}

	void Renderer::clear()
	{
		glClearColor(clearColor(0), clearColor(1), clearColor(2), clearColor(3));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::setClearColor(const uint32_t color)
	{
		clearColor = gl::toVec4(color);
	}

	void Renderer::setRenderTarget(const gl::IFrame& target)
	{
		lockContextPtr()->bindFramebuffer(
			GL_FRAMEBUFFER,
			static_cast<const Framebuffer&>(target).FBO);
	}

	void Renderer::draw(const gl::IDrawable& drawable)
	{
		auto& ptr = static_cast<const Drawable&>(drawable);
		lockContextPtr()->bindVertexArray(ptr.vertexArray->vao);
		// TODO
		if (ptr.nVertices == 0) {
			glDrawArrays(GL_POINTS, 0, 1);
		} else {
			glDrawElements(static_cast<GLenum>(ptr.vertexArray->primitive),
			               ptr.nVertices,
			               ptr.indicesDataType,
			               nullptr);
		}
	}

	void Renderer::draw(const gl::IFrame& buffer)
	{
		lockContextPtr()->bindFramebuffer(GL_FRAMEBUFFER, 0);
		useShader(*unitShader);
		bindTexture(*static_cast<const Framebuffer&>(buffer).texture);
		draw(*unitDrawable);
	}

	void Renderer::draw(const gl::ITexture2D& texture)
	{
		useShader(*unitShader);
		bindTexture(texture);
		draw(*unitDrawable);
	}

	void Renderer::useShader(const gl::IShader& shader)
	{
		lockContextPtr()->bindShader(static_cast<const Shader&>(shader).getId());
	}

	void Renderer::bindTexture(const gl::ITexture2D& texture, const int unit)
	{
		const auto& ptr = static_cast<const Texture2D&>(texture);
		bindTexture(static_cast<const Texture&>(ptr), unit);
	}

	void Renderer::bindTexture(const gl::ICubemap& texture, const int unit)
	{
		const auto& ptr = static_cast<const Cubemap&>(texture);
		bindTexture(static_cast<const Texture&>(ptr), unit);
	}

	Renderer::Renderer(std::weak_ptr<Context> contextPtr)
		: BaseObject(std::move(contextPtr))
	{
		if (!unitDrawable) {
			unitDrawable = lockContextPtr()->getDrawableCreator()->createDrawable(
				UnitShaderTemplate::vertices,
				UnitShaderTemplate::indices,
				UnitShaderTemplate::vertexLayout,
				gl::VertexFormat::INTERLEAVED);
			unitShader = lockContextPtr()->getShaderCreator()->createShader(
				UnitShaderTemplate::vertexCode,
				UnitShaderTemplate::fragmentCode);
			unitShader->setUniform("screenTexture", 0);
		}
	}

	void Renderer::bindTexture(const Texture& texture, const int unit)
	{
		lockContextPtr()->bindTexture(static_cast<GLenum>(texture.target), texture.id, unit);
	}
}
