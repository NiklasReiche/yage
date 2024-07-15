#pragma once
#include "IDescriptorSetCreator.h"
#include "IDescriptorSetLayoutBuilder.h"
#include "IDrawableCreator.h"
#include "IIndexBufferCreator.h"
#include "IPipelineBuilder.h"
#include "IRenderTargetBuilder.h"
#include "IRenderer.h"
#include "ITexture2DCreator.h"
#include "IUniformBufferCreator.h"
#include "IVertexBufferCreator.h"

namespace yage::gl
{
    class IContext
    {
    public:
        virtual ~IContext() = default;

        virtual IRenderer2& renderer() = 0;

        virtual IPipelineBuilder& pipeline_builder() = 0;

        virtual IRenderTargetBuilder& render_target_builder() = 0;

        virtual IDrawableCreator2& drawable_creator() = 0;

        virtual IDescriptorSetLayoutBuilder& descriptor_set_layout_builder() = 0;

        virtual IDescriptorSetCreator& descriptor_set_creator() = 0;

        virtual ITexture2DCreator& texture_2d_creator() = 0;

        virtual IVertexBufferCreator& vertex_buffer_creator() = 0;

        virtual IIndexBufferCreator& index_buffer_creator() = 0;

        virtual IUniformBufferCreator& uniform_buffer_creator() = 0;
    };
}
