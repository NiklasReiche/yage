#include "VertexBufferFactory.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    VertexBufferFactory::VertexBufferFactory(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<IVertexBuffer> VertexBufferFactory::create(const VertexDataInfo& data_info,
                                                      std::span<const std::byte> data)
    {
        const std::size_t vertex_count = vertex_count(data_info, data);
        return m_instance.lock()->store_vertex_buffers()->create(m_instance, data, vertex_count);
    }
}
