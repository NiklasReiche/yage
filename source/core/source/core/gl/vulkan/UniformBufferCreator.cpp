#include "UniformBufferCreator.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    UniformBufferCreator::UniformBufferCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<UniformBuffer> UniformBufferCreator::create(std::uint32_t size, const ResourceUsage usage) const
    {
        const auto instance = m_instance.lock();

        FrameCounter frame_counter{};
        switch (usage) {
            case ResourceUsage::STATIC:  frame_counter = instance->static_counter(); break;
            case ResourceUsage::DYNAMIC: frame_counter = instance->frames_in_flight_counter(); break;

            default: throw std::invalid_argument("unknown ResourceUsage value");
        }

        return instance->store_uniform_buffers()->create(instance.get(), frame_counter, size);
    }
}
