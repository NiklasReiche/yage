#pragma once

#include <string>
#include <vector>
#include <array>
#include <numeric>
#include <sstream>

#include <platform/Platform.h>

#include "gles2.h"
#include "GLES2_Enum.h"
#include "GLES2_State.h"
#include "GLES2_Drawable.h"
#include "GLES2_Framebuffer.h"
#include "GLES2_Shader.h"
#include "GLES2_Texture.h"
#include "GLES2_Viewport.h"
#include "GLES2_UnitShader.h"

namespace gles2
{
	class GLES2_Context
	{
	private:
		platform::PlatformHandle* systemHandle;

		int width, height;

		GLES2_State glState;

		GLES2_Shader unitShader;
		GLES2_Drawable unitDrawable;
		
		InternalFormat convertToInternalFormat(ImageFormat format);
		int convertToChannelSize(ImageFormat format);
		int checkShaderCompilationError(GLuint program, std::string type);

	public:
		GLES2_Context(platform::PlatformHandle* systemHandle);
		~GLES2_Context();

		/*****************************************
		**			Window functions			**
		*****************************************/

		void makeCurrent();
		void swapBuffers();

		int getWidth() { return width; }
		int getHeight() { return height; }

		
		/*****************************************
		**			Enable / Diable				**
		*****************************************/

		void setActiveViewport(const GLES2_Viewport & viewport);
		void setActiveRenderTarget(const GLES2_Framebuffer & framebuffer);
		void setActiveRenderTarget(GLuint FBO);

		void setPackAlignment(int value);
		void setUnpackAlignment(int value);

		void enableDepthTest();
		void disableDepthTest();

		void enableStencilTest();
		void disableStencilTest();

		void enableBlending();
		void disableBlending();


		/*****************************************
		**			Generator functions			**
		*****************************************/

		GLES2_Framebuffer createFramebuffer(
			GLint width,
			GLint height);

		GLES2_Drawable createDrawable(
			std::vector<GLfloat> & vertices,
			std::vector<int> vertexLayout,
			DrawMode usage,
			VertexFormat = VertexFormat::INTERLEAVED);
		void createDrawable(
			GLES2_Drawable & drawable,
			std::vector<GLfloat> & vertices,
			std::vector<int> vertexLayout,
			DrawMode usage,
			VertexFormat format = VertexFormat::INTERLEAVED);

		GLES2_Texture create2DTexture(
			unsigned char* image,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4);
		void create2DTexture(
			GLES2_Texture & texture,
			unsigned char* image,
			int width,
			int height,
			ImageFormat format,
			int rowAlignment = 4);

		GLES2_Texture createCubemapTexture(
			std::array<unsigned char*, 6> images,
			int width,
			int height);
		void createCubemapTexture(
			GLES2_Texture & texture,
			std::array<unsigned char*, 6> images,
			int width,
			int height);

		GLES2_Shader compileShader(
			std::string vertexCode,
			std::string fragmentCode,
			const std::vector<std::string> & attributes);


		/*****************************************
		**			Usage functions				**
		*****************************************/

		void draw(const GLES2_Drawable & drawable);
		void draw(const GLES2_Framebuffer & buffer);

		void useShader(const GLES2_Shader & shader);

		void bindTexture(const GLES2_Texture & texture, int unit = 0);
	};
}