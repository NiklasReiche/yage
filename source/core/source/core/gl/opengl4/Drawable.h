#pragma once

#include <glad/gl.h>

#include "../IDrawable.h"
#include "../IIndexBuffer.h"
#include "../IVertexBuffer.h"

namespace yage::gl::opengl4
{
    class Context;

    struct DrawableDescriptor
    {
        VertexBufferHandle vertex_buffer;
        std::size_t vertex_buffer_offset;
        std::size_t n_vertices;
        VertexDataInfo vertex_data_info;

        IndexBufferHandle index_buffer;
        std::size_t index_buffer_offset;
        IndexDataInfo index_data_info;
    };

    class Drawable : public IDrawable2
    {
    public:
        Drawable(Context* context, DrawableDescriptor descriptor);

        ~Drawable() override;

        Drawable(const Drawable& other) = delete;

        Drawable(Drawable&& other) noexcept;

        Drawable& operator=(const Drawable& other) = delete;

        Drawable& operator=(Drawable&& other) noexcept;

        [[nodiscard]] GLuint gl_vao_handle() const;

        [[nodiscard]] GLenum gl_index_type() const;

        [[nodiscard]] std::int32_t indices_count() const;

    private:
        Context* m_context;

        GLuint m_vao_handle = 0;

        VertexBufferHandle m_vertex_buffer;
        IndexBufferHandle m_index_buffer;

        std::int32_t m_indices_count;
        GLenum m_indices_data_type;

        void clear();
    };
}
