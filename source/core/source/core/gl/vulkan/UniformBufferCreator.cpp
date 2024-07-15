#include "UniformBufferCreator.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    UniformBufferCreator::UniformBufferCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    UniformBufferHandle UniformBufferCreator::create(std::size_t size, const ResourceUsage usage) const
    {
        const auto instance = m_instance.lock();
        return instance->store_uniform_buffers().create(instance.get(), instance->frame_counter_for_usage(usage), size);
    }
}
