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

        FrameCounter frame_counter{};
        switch (usage) {
            case ResourceUsage::STATIC:  frame_counter = m_instance.lock()->static_counter(); break;
            case ResourceUsage::DYNAMIC: frame_counter = m_instance.lock()->frames_in_flight_counter(); break;

            default: throw std::invalid_argument("unkown ResourceUsage value");
        }

        return instance->store_textures().create(instance.get(), frame_counter, sampler, data_info, data);
    }
}
