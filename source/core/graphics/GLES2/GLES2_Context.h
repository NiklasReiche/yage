#pragma once

#include <vector>
#include <string>
#include <sstream>

#include <platform/Platform.h>

#include "gles2.h"
#include "GLES2_Drawable.h"
#include "GLES2_Framebuffer.h"
#include "GLES2_Shader.h"
#include "GLES2_Texture.h"
#include "GLES2_Viewport.h"

namespace gles2
{
	class GLES2_Context
	{
	private:
		platform::PlatformHandle* systemHandle;

		int width, height;
		
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

		void enableWireframe();
		void disableWireframe();


		/*****************************************
		**			Generator functions			**
		*****************************************/

		GLES2_Shader compileShader(
			std::string vertexCode,
			std::string fragmentCode,
			std::string geometryCode = "");


		/*****************************************
		**			Usage functions				**
		*****************************************/

		void draw(const GLES2_Drawable & drawable);
		void draw(const GLES2_Framebuffer & buffer);

		void useShader(const GLES2_Shader & shader);

		void bindTexture(const GLES2_Texture & texture, int unit = 0);
	};
}