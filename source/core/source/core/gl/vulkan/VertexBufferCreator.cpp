#include "VertexBufferCreator.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    VertexBufferCreator::VertexBufferCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    VertexBufferHandle VertexBufferCreator::create(const VertexDataInfo& data_info, std::span<const std::byte> data,
                                                   const ResourceUsage usage) const
    {
        const auto instance = m_instance.lock();
        return instance->store_vertex_buffers().create(instance.get(), instance->frame_counter_for_usage(usage),
                                                       data_info, data);
    }
}
