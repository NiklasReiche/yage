#include "DrawableCreator.h"

#include "IndexBufferCreator.h"
#include "Instance.h"
#include "VertexBufferCreator.h"

namespace yage::gl::vulkan
{
    DrawableCreator::DrawableCreator(std::weak_ptr<Instance> m_instance)
        : m_instance(std::move(m_instance))
    {
    }

    Handle<IDrawable2> DrawableCreator::create(const VertexDataInfo vertex_data_info,
                                               const std::span<const std::byte> vertices,
                                               const IndexDataInfo index_data_info,
                                               const std::span<const std::byte> indices,
                                               const ResourceUsage usage) const
    {
        DrawableDescriptor descriptor{
                .vertex_buffer = VertexBufferCreator(m_instance).create(vertex_data_info, vertices, usage),
                .vertex_buffer_offset = 0,
                .vertex_data_info = vertex_data_info,
                .index_buffer = IndexBufferCreator(m_instance).create(index_data_info, indices, usage),
                .index_buffer_offset = 0,
                .index_data_info = index_data_info};

        const auto instance = m_instance.lock();
        return instance->store_drawables().create(descriptor);
    }
}
