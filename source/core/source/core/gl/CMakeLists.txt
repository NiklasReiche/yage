target_sources(yage_core
	PRIVATE
		color.h
		Context.h
		Cubemap.h
		Drawable.h
		DrawableCreator.h
		Frame.h
		FrameCreator.h
		graphics.h
		ImageFormat.h
		PixelTransfer.h
		Renderer.h
		Shader.h
		ShaderCreator.h
		Texture2D.h
		TextureCreator.h)

if(WIN32 OR UNIX)
	add_subdirectory(opengl3)
elseif(ANDROID)
	add_subdirectory(opengles2)
endif()
