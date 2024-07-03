#include "VertexBufferFactory.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    VertexBufferFactory::VertexBufferFactory(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<IVertexBuffer> VertexBufferFactory::create_test(const std::span<const std::byte>& data,
                                                           std::size_t vertex_count)
    {
        return m_instance.lock()->store_vertex_buffers()->create(m_instance, data, vertex_count);
    }
}
