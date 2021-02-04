target_sources(yage_core PRIVATE
	OpenGL.h

	GlException.h
	GlEnum.h
	BaseObject.h
	BaseObject.cpp
	OpenGlObject.h
	OpenGlObject.cpp

	Drawable.h
	Drawable.cpp
	DrawableCreator.h
	DrawableCreator.cpp

	Framebuffer.h
	Framebuffer.cpp
	FrameCreator.h
	FrameCreator.cpp

	Shader.h
	Shader.cpp
	ShaderCreator.h
	ShaderCreator.cpp
	UniformBuffer.h
	UniformBuffer.cpp

	Texture.h
	Texture.cpp
	Texture2D.h
	Texture2D.cpp
	Cubemap.h
	Cubemap.cpp
	TextureCreator.h
	TextureCreator.cpp

	UnitShader.h

	Context.h
	Context.cpp
	Renderer.h
	Renderer.cpp
	)