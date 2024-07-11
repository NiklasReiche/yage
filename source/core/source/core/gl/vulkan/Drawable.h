#pragma once

#include "../IDrawable.h"
#include "../Handle.h"
#include "../IIndexBuffer.h"
#include "../IVertexBuffer.h"

namespace yage::gl::vulkan
{
    struct DrawableDescriptor
    {
        VertexBufferHandle vertex_buffer;
        std::size_t vertex_buffer_offset;
        VertexDataInfo vertex_data_info;
        IndexBufferHandle index_buffer;
        std::size_t index_buffer_offset;
        IndexDataInfo index_data_info;
    };

    class Drawable final : public IDrawable2
    {
    public:
        explicit Drawable(DrawableDescriptor descriptor);

        [[nodiscard]] const DrawableDescriptor& descriptor() const;

        [[nodiscard]] VertexDataInfo vertex_data_info() const override;

    private:
        DrawableDescriptor m_descriptor;
    };
}
