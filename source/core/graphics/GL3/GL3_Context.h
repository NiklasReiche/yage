#pragma once

#include <string>
#include <vector>
#include <array>
#include <numeric>
#include <memory>

#include "gl3.h"
#include "GL3_Exception.h"
#include "GL3_Enum.h"
#include "GL3_State.h"

#include "GL3_Object.h"

#include "GL3_Shader.h"
#include "GL3_UnitShader.h"

#include "GL3_Drawable.h"
#include "GL3_Framebuffer.h"
#include "GL3_Viewport.h"
#include "GL3_Texture.h"

#include <math/MVector.h>
#include <platform/platform.h>

namespace gl3
{
	void glfw_error_callback(int error, const char* description);
	void gl_error_callback(GLenum error);

	void APIENTRY error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

	class GL3_Context
	{
	private:
		sys::PlatformHandle* systemHandle;

		GLuint width;
		GLuint height;
		std::string title;

		GL3_State glState;

		GL3_Shader unitShader;
		GL3_Shader unitTransformShader;
		GL3_Drawable unitDrawable;

		int checkShaderCompilationError(GLuint program, std::string type);
		InternalFormat convertToInternalFormat(ImageFormat format);
		int convertToChannelSize(ImageFormat format);

	public:
		GL3_Context(sys::PlatformHandle* systemHandle, int width, int height, std::string title = "OpenGL");
		~GL3_Context();


		/*****************************************
		**			Window functions			**
		*****************************************/

		void makeCurrent();
		void showWindow();
		void hideWindow();
		void swapBuffers();

		int getWidth() { return width; }
		int getHeight() { return height; }
		int getCloseFlag() { return systemHandle->shouldDestroy(); }

		GL3_State getGlState() { return glState; }


		/*****************************************
		**			Enable / Diable				**
		*****************************************/

		void setActiveViewport(const Viewport & viewport);
		void setActiveRenderTarget(const GL3_Framebuffer & framebuffer);
		void setActiveRenderTarget(GLuint FBO);

		void setPackAlignment(int value);
		void setUnpackAlignment(int value);

		void enableDepthTest();
		void disableDepthTest();

		void enableStencilTest();
		void disableStencilTest();

		void enableBlending();
		void disableBlending();

		void enableWireframe();
		void disableWireframe();

		void enableVSync();
		void disableVSync();


		/*****************************************
		**			Generator functions			**
		*****************************************/

		GL3_Framebuffer createFramebuffer(
			GLint width,
			GLint height,
			ImageFormat format = ImageFormat::RGB);

		GL3_Drawable createDrawable(
			std::vector<GLfloat> & vertices,
			std::vector<int> vertexLayout,
			DrawMode usage,
			VertexFormat = VertexFormat::INTERLEAVED);
		void createDrawable(
			GL3_Drawable & drawable,
			std::vector<GLfloat> & vertices,
			std::vector<int> vertexLayout,
			DrawMode usage,
			VertexFormat format = VertexFormat::INTERLEAVED);

		GL3_Texture create2DTexture(
			unsigned char* image,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4);
		void create2DTexture(
			GL3_Texture & texture,
			unsigned char* image,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4);

		GL3_Texture createCubemapTexture(
			std::array<unsigned char*, 6> images,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4);
		void createCubemapTexture(
			GL3_Texture & texture,
			std::array<unsigned char*, 6> images,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4);

		GL3_Shader compileShader(
			std::string vertexCode,
			std::string fragmentCode,
			std::string geometryCode = "");


		/*****************************************
		**			Usage functions				**
		*****************************************/

		void draw(const GL3_Drawable & drawable);
		void draw(const GL3_Framebuffer & buffer);
		void draw(const GL3_Texture & texture);

		void useShader(const GL3_Shader & shader);

		void bindTexture(const GL3_Texture & texture, int unit = 0);

		void useUnitShader();
		void useUnitTransformShader(gml::Matrix4D<float> transform);
		void drawUnitDrawable();
	};
}
