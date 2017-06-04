#pragma once

#include <string>
#include <vector>
#include <array>
#include <numeric>

#include "gl3.h"
#include "GL3_Exception.h"
#include "GL3_Enum.h"
#include "GL3_State.h"

#include "GL3_Drawable.h"
#include "GL3_Framebuffer.h"
#include "GL3_Shader.h"
#include "GL3_Viewport.h"
#include "GL3_Texture.h"

#include "math\MVector.h"

namespace gl3
{
	class GL3_Shader;

	void error_callback(int error, const char* description);

	class GL3_Context
	{
	private:
		GLFWwindow* glfwHandle;
		const int GL_VERSION_MAJOR = 3;
		const int GL_VERSION_MINOR = 3;

		GLuint width;
		GLuint height;
		std::string title;

		GL3_State glState;

		int checkShaderCompilationError(GLuint program, std::string type);
		InternalFormat convertToInternalFormat(ImageFormat format);
		int convertToChannelSize(ImageFormat format);
	public:
		GL3_Context(int width, int height, std::string title = "OpenGL");
		~GL3_Context();


		/*****************************************
		**			Window functions			**
		*****************************************/

		void makeCurrent();
		void showWindow();
		void hideWindow();
		void swapBuffers();

		GLFWwindow* getGlfwHandle() { return glfwHandle; }
		int getWidth() { return width; }
		int getHeight() { return height; }
		int getCloseFlag() { return glfwWindowShouldClose(glfwHandle); }


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


		/*****************************************
		**			Generator functions			**
		*****************************************/

		GL3_Framebuffer createFramebuffer(
			GLint width,
			GLint height,
			gml::Vec4<float> clearColor = gml::Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f));

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
			int height);
		void createCubemapTexture(
			GL3_Texture & texture,
			std::array<unsigned char*, 6> images,
			int width,
			int height);

		GL3_Shader compileShader(
			std::string vertexCode,
			std::string fragmentCode,
			std::string geometryCode = "");


		/*****************************************
		**			Usage functions				**
		*****************************************/

		void draw(GL3_Drawable & drawable);
		void draw(GL3_Framebuffer & buffer);
		void useShader(GL3_Shader & shader);
		void bindTexture(GL3_Texture & texture, int unit);
	};
}