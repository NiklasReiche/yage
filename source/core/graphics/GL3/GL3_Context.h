#pragma once

#include <memory>
#include <map>

#include "GL3_Enum.h"

#include <platform/OpenGL.h>
#include <platform/desktop/GlfwWindow.h>
#include "../Context.h"

namespace gl3
{
	void APIENTRY onGlError(
		GLenum source, GLenum type, GLuint id, GLenum severity, 
		GLsizei length, const GLchar* message, const void* userParam);

	class GL3_Context : public gl::IContext, public std::enable_shared_from_this<GL3_Context>
	{
	public:
		explicit GL3_Context(sys::desktop::GlfwWindow* window);
		~GL3_Context() = default;

		void makeCurrent();

		std::shared_ptr<gl::IDrawableCreator> getDrawableCreator() override;
		std::shared_ptr<gl::ITextureCreator> getTextureCreator() override;
		std::shared_ptr<gl::IFrameCreator> getFrameCreator() override;
		std::shared_ptr<gl::IShaderCreator> getShaderCreator() override;
		std::shared_ptr<gl::IRenderer> getRenderer() override;


		/*****************************************
		**			Enable / Disable			**
		*****************************************/

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

		static InternalFormat convertToInternalFormat(gl::ImageFormat format);
		static ImageFormat convertToImageFormat(gl::ImageFormat format);
		static int convertToChannelSize(ImageFormat format);

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
		
		sys::desktop::GlfwWindow* window;

		std::shared_ptr<gl::IRenderer> renderer;
		std::shared_ptr<gl::IShaderCreator> shaderCreator;
		std::shared_ptr<gl::IDrawableCreator> drawableCreator;
		std::shared_ptr<gl::ITextureCreator> textureCreator;
		std::shared_ptr<gl::IFrameCreator> frameCreator;
	};
}
