#include "IndexBufferCreator.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    IndexBufferCreator::IndexBufferCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<IIndexBuffer> IndexBufferCreator::create(IndexDataInfo data_info, std::span<const std::byte> data) const
    {
        return m_instance.lock()->store_index_buffers()->create(m_instance, data_info, data);
    }
}
