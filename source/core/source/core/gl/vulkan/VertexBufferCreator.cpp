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
        const auto instance = m_instance.lock();
        return instance->store_vertex_buffers().create(instance.get(), data_info, data);
    }
}
