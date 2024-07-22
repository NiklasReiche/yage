#include "RenderTargetBuilder.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    RenderTargetBuilder::RenderTargetBuilder(Context* context)
        : m_context(context)
    {
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_msaa(const MSAASamples samples)
    {
        m_samples = samples;

        return *this;
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_color_attachment(const TextureFormat2 format)
    {
        m_color_attachments.push_back(format);

        return *this;
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_depth_attachment(TextureFormat2 format)
    {
        if (!has_depth_component(format)) {
            throw std::invalid_argument("Invalid texture format for depth attachment");
        }

        m_depth_attachment = format;

        return *this;
    }

    IRenderTargetBuilder& RenderTargetBuilder::with_resolve_attachments()
    {
        m_with_resolve = true;

        return *this;
    }

    RenderTargetHandle RenderTargetBuilder::build(unsigned width, unsigned height, ResourceUsage)
    {
        return m_context->store_render_targets().create(m_context, width, height, m_samples, m_color_attachments,
                                                        m_depth_attachment, m_with_resolve);
    }

    void RenderTargetBuilder::clear()
    {
        m_samples = MSAASamples::SAMPLE_1;
        m_color_attachments.clear();
        m_depth_attachment.reset();
        m_with_resolve = false;
    }
}
