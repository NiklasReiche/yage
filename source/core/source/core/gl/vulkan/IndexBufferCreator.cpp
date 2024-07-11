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
        const auto instance = m_instance.lock();
        return instance->store_index_buffers().create(instance.get(), data_info, data);
    }
}
