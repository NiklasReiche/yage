#include "Instance.h"
#include "VertexBufferCreator.h"

namespace yage::gl::vulkan
{
    VertexBufferCreator::VertexBufferCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<IVertexBuffer> VertexBufferCreator::create(const VertexDataInfo& data_info,
                                                      std::span<const std::byte> data) const
    {
        return m_instance.lock()->store_vertex_buffers()->create(m_instance, data_info, data);
    }
}
