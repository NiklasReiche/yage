#include "Texture2DCreator.h"

#include "Instance.h"

namespace yage::gl::vulkan
{
    Texture2DCreator::Texture2DCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Texture2DHandle Texture2DCreator::create(const TextureSampler& sampler, const PixelTransferInfo& data_info,
                                             const std::span<const std::byte> data, const ResourceUsage usage) const
    {
        const auto instance = m_instance.lock();
        return instance->store_textures().create(instance.get(), instance->frame_counter_for_usage(usage), sampler,
                                                 data_info, data);
    }

    Texture2DHandle Texture2DCreator::create_multisampled(MSAASamples, const PixelTransferInfo&,
                                                          ResourceUsage) const
    {
        // TODO
        return Texture2DHandle();
    }
}
