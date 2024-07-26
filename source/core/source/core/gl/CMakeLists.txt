target_sources(yage_core
	PRIVATE
		api.h

		Handle.h

		color.h
		enums.h

		IDescriptorSetLayout.h
		IDescriptorSetLayoutBuilder.h
		IDescriptorSet.h
		IDescriptorSetCreator.h

		IRenderTarget.h
		IRenderTargetBuilder.h

		IContext.h

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
	add_subdirectory(opengl4)
	add_subdirectory(vulkan)
elseif(ANDROID)
	add_subdirectory(opengles2)
endif()
