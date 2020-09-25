target_sources(yage_core PRIVATE
	OpenGL.h

	GL3_Exception.h
	GL3_Enum.h
	GL3_BaseObject.h
	GL3_Object.h
	GL3_Object.cpp

	GL3_Drawable.h
	GL3_Drawable.cpp
	GL3_DrawableCreator.h
	GL3_DrawableCreator.cpp

	GL3_Framebuffer.h
	GL3_Framebuffer.cpp
	GL3_FrameCreator.h
	GL3_FrameCreator.cpp

	GL3_Shader.h
	GL3_Shader.cpp
	GL3_ShaderCreator.h
	GL3_ShaderCreator.cpp

	GL3_Texture.h
	GL3_Texture.cpp
	GL3_Texture2D.h
	GL3_Texture2D.cpp
	GL3_Cubemap.h
	GL3_Cubemap.cpp
	GL3_TextureCreator.h
	GL3_TextureCreator.cpp

	GL3_UnitShader.h

	GL3_Context.h
	GL3_Context.cpp
	GL3_Renderer.h
	GL3_Renderer.cpp
	)