target_sources(yage_core PRIVATE
        enums.h

        Context.h
        Context.cpp

        DescriptorSet.h
        DescriptorSet.cpp
        DescriptorSetCreator.h
        DescriptorSetCreator.cpp

        DescriptorSetLayout.h
        DescriptorSetLayout.cpp
        DescriptorSetLayoutBuilder.h
        DescriptorSetLayoutBuilder.cpp

        Drawable.h
        Drawable.cpp
        DrawableCreator.h
        DrawableCreator.cpp

        IndexBuffer.h
        IndexBuffer.cpp
        IndexBufferCreator.h
        IndexBufferCreator.cpp

        VertexBuffer.h
        VertexBuffer.cpp
        VertexBufferCreator.h
        VertexBufferCreator.cpp

        UniformBuffer.h
        UniformBuffer.cpp
        UniformBufferCreator.h
        UniformBufferCreator.cpp

        Texture2D.h
        Texture2D.cpp
        Texture2DCreator.h
        Texture2DCreator.cpp

        Pipeline.h
        Pipeline.cpp
        PipelineBuilder.h
        PipelineBuilder.cpp

        RenderTarget.h
        RenderTarget.cpp
        RenderTargetBuilder.h
        RenderTargetBuilder.cpp

        Renderer.h
        Renderer.cpp
)