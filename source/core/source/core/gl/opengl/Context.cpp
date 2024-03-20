#include "Context.h"

#include <algorithm>

#include <GLFW/glfw3.h>

#include <core/platform/Window.h>

#include "DrawableCreator.h"
#include "FrameCreator.h"
#include "GlException.h"
#include "Renderer.h"
#include "ShaderCreator.h"
#include "TextureCreator.h"

namespace opengl
{
	void APIENTRY onGlError(
	    const GLenum, const GLenum, const GLuint id, const GLenum severity,
	    const GLsizei, const GLchar* message, const void*)
	{
		if (severity == GL_DEBUG_SEVERITY_HIGH) {
			throw GlException(id, std::string(message));
		}
	}

	Context::Context(const std::weak_ptr<platform::IWindow>& window)
	    : window(window)
	{
		auto windowLock = window.lock();
		if (!windowLock)
			throw std::invalid_argument("Failed to get lock for window during opengl context creation");

		// --- GLAD ---
		if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress))) {
			throw GlException(GLAD_ERROR, "Failed to initialize GLAD");
		}

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLDEBUGPROC(onGlError), nullptr);

		glState.renderTargetWidth = windowLock->getPixelWidth();
		glState.renderTargetHeight = windowLock->getPixelHeight();

		glState.viewportWidth = glState.renderTargetWidth;
		glState.viewportHeight = glState.renderTargetHeight;

		makeCurrent();
	}

	void Context::makeCurrent()
	{
		auto windowLock = window.lock();
		if (!windowLock)
			throw std::logic_error("Failed to get lock for window during opengl context operation");
		windowLock->makeCurrent();
	}

	std::shared_ptr<gl::IDrawableCreator> Context::getDrawableCreator()
	{
		if (drawableCreator == nullptr) {
			drawableCreator = std::shared_ptr<DrawableCreator>(new DrawableCreator(shared_from_this()));
		}
		return drawableCreator;
	}

	std::shared_ptr<gl::ITextureCreator> Context::getTextureCreator()
	{
		if (textureCreator == nullptr) {
			textureCreator = std::shared_ptr<TextureCreator>(new TextureCreator(shared_from_this()));
		}
		return textureCreator;
	}

	std::shared_ptr<gl::IFrameCreator> Context::getFrameCreator()
	{
		if (frameCreator == nullptr) {
			frameCreator = std::shared_ptr<FrameCreator>(new FrameCreator(shared_from_this()));
		}
		return frameCreator;
	}

	std::shared_ptr<gl::IShaderCreator> Context::getShaderCreator()
	{
		if (shaderCreator == nullptr) {
			shaderCreator = std::shared_ptr<ShaderCreator>(new ShaderCreator(shared_from_this()));
		}
		return shaderCreator;
	}

	std::shared_ptr<gl::IRenderer> Context::getRenderer()
	{
		return std::shared_ptr<Renderer>(new Renderer(shared_from_this()));
	}


	void Context::setPackAlignment(const int value)
	{
		if (value != glState.packAlignment) {
			glPixelStorei(GL_PACK_ALIGNMENT, value);
			glState.packAlignment = value;
		}
	}
	void Context::setUnpackAlignment(const int value)
	{
		if (value != glState.unpackAlignment) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, value);
			glState.unpackAlignment = value;
		}
	}

	void Context::setViewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
	{
		if (glState.viewportX != x ||
		    glState.viewportY != y ||
		    glState.viewportWidth != width ||
		    glState.viewportHeight != height) {
			glViewport(x, y, width, height);
			glState.viewportX = x;
			glState.viewportY = y;
			glState.viewportWidth = width;
			glState.viewportHeight = height;
		}
	}

	void Context::bindBuffer(const GLenum target, const GLuint buffer)
	{
		const auto it = glState.buffers.find(target);
		if (it != glState.buffers.end() && it->second == buffer)
			return;

		glBindBuffer(target, buffer);
		glState.buffers[target] = buffer;
	}

	void Context::bindFramebuffer(const GLenum target, const GLuint framebuffer)
	{
		const auto it = glState.framebuffers.find(target);
		if (it != glState.framebuffers.end() && it->second == framebuffer)
			return;

		glBindFramebuffer(target, framebuffer);
		glState.framebuffers[target] = framebuffer;

		if (target == GL_FRAMEBUFFER) {
			glState.framebuffers[GL_DRAW_FRAMEBUFFER] = framebuffer;
			glState.framebuffers[GL_READ_FRAMEBUFFER] = framebuffer;
		}
	}

	void Context::bindVertexArray(const GLuint array)
	{
		if (glState.vao == array)
			return;

		glBindVertexArray(array);
		glState.vao = array;
	}

	void Context::bindTexture(const GLenum target, const GLuint texture, const int unit)
	{
		if (glState.activeTextureUnit != unit) {
			glActiveTexture(GL_TEXTURE0 + unit);
			glState.activeTextureUnit = unit;
		}

		auto textures = glState.textures[unit];

		const auto it = textures.find(target);
		if (it != textures.end() && it->second == texture)
			return;

		glBindTexture(target, texture);
		glState.textures[unit][target] = texture;
	}

	void Context::bindShader(const GLuint shader)
	{
		if (glState.shader == shader)
			return;

		glUseProgram(shader);
		glState.shader = shader;
	}

	void Context::linkUbo(GLuint ubo)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, glState.uniformBuffers.size(), ubo);
		glState.uniformBuffers.push_back(ubo);
	}

	int Context::getUboBindPoint(GLuint ubo)
	{
		auto it = std::find(glState.uniformBuffers.begin(), glState.uniformBuffers.end(), ubo);

		if (it == glState.uniformBuffers.end())
			throw std::exception(); // TODO

		return it - glState.uniformBuffers.begin();
	}

	void Context::enableDepthTest(const GLenum func, const GLboolean flag)
	{
		if (!glState.isDepthTestEnabled) {
			glEnable(GL_DEPTH_TEST);
			glState.isDepthTestEnabled = true;
		}
		if (glState.depthFunc != func) {
			glDepthFunc(func);
			glState.depthFunc = func;
		}
		if (glState.depthMask != flag) {
			glDepthMask(flag);
			glState.depthMask = flag;
		}
	}

	void Context::disableDepthTest()
	{
		if (glState.isDepthTestEnabled) {
			glDisable(GL_DEPTH_TEST);
			glState.isDepthTestEnabled = false;
		}
	}

	void Context::enableStencilTest()
	{
		if (!glState.isStencilTestEnabled) {
			glEnable(GL_STENCIL_TEST);
			glState.isStencilTestEnabled = true;
		}
	}

	void Context::disableStencilTest()
	{
		if (glState.isStencilTestEnabled) {
			glDisable(GL_STENCIL_TEST);
			glState.isStencilTestEnabled = false;
		}
	}

	void Context::enableBlending(const GLenum sFactor, const GLenum dFactor)
	{
		if (!glState.isBlendingEnabled) {
			glEnable(GL_BLEND);
			glState.isBlendingEnabled = true;
		}

		if (glState.blendFuncFactorS != sFactor || glState.blendFuncFactorD != dFactor) {
			glBlendFunc(sFactor, dFactor);
			glState.blendFuncFactorS = sFactor;
			glState.blendFuncFactorD = dFactor;
		}
	}

	void Context::disableBlending()
	{
		if (glState.isBlendingEnabled) {
			glDisable(GL_BLEND);
			glState.isBlendingEnabled = false;
		}
	}

	void Context::setPolygonMode(const GLenum mode)
	{
		if (glState.polygonMode != mode) {
			glPolygonMode(GL_FRONT_AND_BACK, mode);
			glState.polygonMode = mode;
		}
	}

	int Context::getRenderTargetWidth() const
	{
		return glState.renderTargetWidth;
	}

	int Context::getRenderTargetHeight() const
	{
		return glState.renderTargetHeight;
	}
}// namespace gl3

namespace gl
{
	std::shared_ptr<IContext> createContext(const std::weak_ptr<platform::IWindow>& window)
	{
		return std::make_shared<opengl::Context>(window);
	}
}// namespace gl
