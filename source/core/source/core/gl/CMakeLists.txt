target_sources(yage_core
	PRIVATE
		Handle.h
		color.h
		Context.h
		Cubemap.h
		Drawable.h
		VertexArray.h
		VertexBuffer.h
		ElementBuffer.h
		DrawableCreator.h
		Frame.h
		FrameCreator.h
		graphics.h
        TextureParams.h
		Renderer.h
		Shader.h
		ShaderCreator.h
		Texture2D.h
		TextureCreator.h

		enums.h

		IDescriptorSetLayout.h
		IDescriptorSetLayoutBuilder.h
		IDescriptorSet.h
		IDescriptorSetCreator.h

		IRenderTarget.h
		IRenderTargetBuilder.h

		IRenderer.h

		IPipeline.h
		IPipelineBuilder.h

		IVertexBuffer.h
		IVertexBufferCreator.h

		IIndexBuffer.h
		IIndexBufferCreator.h

		IUniformBuffer.h
		IUniformBufferCreator.h

		IDrawable.h
		IDrawableCreator.h

		ITexture2D.h
		ITexture2DCreator.h
)

if(WIN32 OR UNIX)
	add_subdirectory(opengl)
	target_compile_definitions(yage_core PRIVATE "OPENGL")

	add_subdirectory(vulkan)
elseif(ANDROID)
	add_subdirectory(opengles2)
endif()
