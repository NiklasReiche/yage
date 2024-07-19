#include "Texture2DCreator.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    Texture2DCreator::Texture2DCreator(Context* context)
        : m_context(context)
    {
    }

    Texture2DHandle Texture2DCreator::create(const TextureSampler& sampler, const PixelTransferInfo& data_info,
                                             std::span<const std::byte> data, ResourceUsage) const
    {
        return m_context->store_texture2ds().create(m_context, sampler, data_info, data);
    }

    Texture2DHandle Texture2DCreator::create_multisampled(const MSAASamples samples, const PixelTransferInfo& data_info,
                                                          ResourceUsage) const
    {
        return m_context->store_texture2ds().create(m_context, samples, data_info);
    }
}
