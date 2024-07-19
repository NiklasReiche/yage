#include "DrawableCreator.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    DrawableCreator::DrawableCreator(Context* context)
        : m_context(context)
    {
    }

    DrawableHandle DrawableCreator::create(const VertexDataInfo vertex_data_info,
                                           const std::span<const std::byte> vertices,
                                           const IndexDataInfo index_data_info,
                                           const std::span<const std::byte> indices, const ResourceUsage usage) const
    {
        DrawableDescriptor descriptor{
                .vertex_buffer = m_context->vertex_buffer_creator().create(vertex_data_info, vertices, usage),
                .vertex_buffer_offset = 0,
                .n_vertices = vertex_count(vertex_data_info, vertices),
                .vertex_data_info = vertex_data_info,

                .index_buffer = m_context->index_buffer_creator().create(index_data_info, indices, usage),
                .index_buffer_offset = 0,
                .index_data_info = index_data_info,
        };
        return m_context->store_drawables().create(m_context, descriptor);
    }
}
