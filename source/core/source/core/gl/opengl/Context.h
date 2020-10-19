#pragma once

#include <memory>
#include <map>

#include <glad/glad.h>

#include <core/gl/Context.h>

#include "GlEnum.h"

namespace platform
{
	class IWindow;
}

namespace opengl
{
	void APIENTRY onGlError(
		GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* userParam);

	class Context : public gl::IContext, public std::enable_shared_from_this<Context>
	{
	public:
		explicit Context(const std::weak_ptr<platform::IWindow>& window);
		~Context() override = default;

		void makeCurrent();

		std::shared_ptr<gl::IDrawableCreator> getDrawableCreator() override;
		std::shared_ptr<gl::ITextureCreator> getTextureCreator() override;
		std::shared_ptr<gl::IFrameCreator> getFrameCreator() override;
		std::shared_ptr<gl::IShaderCreator> getShaderCreator() override;
		std::shared_ptr<gl::IRenderer> getRenderer() override;


		void setPackAlignment(int value);
		void setUnpackAlignment(int value);

		int getRenderTargetWidth() const;
		int getRenderTargetHeight() const;

		void setViewport(GLint x, GLint y, GLsizei width, GLsizei height);
		
		void bindBuffer(GLenum target, GLuint buffer);
		void bindFramebuffer(GLenum target, GLuint framebuffer);
		void bindVertexArray(GLuint array);
		void bindTexture(GLenum target, GLuint texture, int unit = 0);
		void bindShader(GLuint shader);

		void enableDepthTest(GLenum func, GLboolean flag);
		void disableDepthTest();
		
		void enableStencilTest();
		void disableStencilTest();
		
		void enableBlending(GLenum sFactor, GLenum dFactor);
		void disableBlending();
		
		void setPolygonMode(GLenum mode);

	private:
		struct OpenGlState
		{
			GLuint shader = -1;
			GLuint renderTarget = 0;
			GLuint vao = 0;

			int renderTargetWidth = -1;
			int renderTargetHeight = -1;

			int viewportX = 0;
			int viewportY = 0;
			int viewportWidth = -1;
			int viewportHeight = -1;

			int activeTextureUnit = 0;
			std::map<int, std::map<GLenum, GLuint>> textures;
			std::map<GLenum, GLuint> buffers;
			std::map<GLenum, GLuint> framebuffers;

			int packAlignment = 4;
			int unpackAlignment = 4;

			bool isDepthTestEnabled = false;
			GLenum depthFunc = GL_LESS;
			GLboolean depthMask = GL_TRUE;

			bool isStencilTestEnabled = false;
			
			bool isBlendingEnabled = false;
			GLenum blendFuncFactorD = GL_ZERO;
			GLenum blendFuncFactorS = GL_ONE;

			GLenum polygonMode = GL_FILL;
		};

		OpenGlState glState;
		
		std::weak_ptr<platform::IWindow> window;

		std::shared_ptr<gl::IRenderer> renderer;
		std::shared_ptr<gl::IShaderCreator> shaderCreator;
		std::shared_ptr<gl::IDrawableCreator> drawableCreator;
		std::shared_ptr<gl::ITextureCreator> textureCreator;
		std::shared_ptr<gl::IFrameCreator> frameCreator;
	};
}
