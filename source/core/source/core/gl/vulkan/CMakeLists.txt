target_sources(yage_core PRIVATE
        enums.h
        Instance.h
        Instance.cpp

        VertexBuffer.h
        VertexBuffer.cpp
        VertexBufferCreator.h
        VertexBufferCreator.cpp

        IndexBuffer.h
        IndexBuffer.cpp
        IndexBufferCreator.h
        IndexBufferCreator.cpp

        Drawable.h
        Drawable.cpp
        DrawableCreator.h
        DrawableCreator.cpp

        Texture2D.h
        Texture2D.cpp
        Texture2DCreator.h
        Texture2DCreator.cpp

        Pipeline.h
        Pipeline.cpp
        PipelineBuilder.h
        PipelineBuilder.cpp

        FrameBuffer.h
        FrameBuffer.cpp
        FrameBufferCreator.h
        FrameBufferCreator.cpp

        RenderPass.h
        RenderPass.cpp
        RenderPassCreator.h
        RenderPassCreator.cpp

        ImageView.h
        ImageView.cpp
        Renderer.h
        Renderer.cpp
)