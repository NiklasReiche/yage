target_sources(yage_gl
	PUBLIC GL3_Exception.h
	PUBLIC GL3_Enum.h
	PUBLIC GL3_BaseObject.h
	PUBLIC GL3_Object.h
	PUBLIC GL3_Object.cpp

	PUBLIC GL3_Drawable.h
	PUBLIC GL3_Drawable.cpp
	PUBLIC GL3_DrawableCreator.h
	PUBLIC GL3_DrawableCreator.cpp

	PUBLIC GL3_Framebuffer.h
	PUBLIC GL3_Framebuffer.cpp
	PUBLIC GL3_FrameCreator.h
	PUBLIC GL3_FrameCreator.cpp

	PUBLIC GL3_Shader.h
	PUBLIC GL3_Shader.cpp
	PUBLIC GL3_ShaderCreator.h
	PUBLIC GL3_ShaderCreator.cpp

	PUBLIC GL3_Texture.h
	PUBLIC GL3_Texture.cpp
	PUBLIC GL3_Texture2D.h
	PUBLIC GL3_Texture2D.cpp
	PUBLIC GL3_Cubemap.h
	PUBLIC GL3_Cubemap.cpp
	PUBLIC GL3_TextureCreator.h
	PUBLIC GL3_TextureCreator.cpp

	PUBLIC GL3_UnitShader.h

	PUBLIC GL3_Context.h
	PUBLIC GL3_Context.cpp
	PUBLIC GL3_Renderer.h
	PUBLIC GL3_Renderer.cpp
	)