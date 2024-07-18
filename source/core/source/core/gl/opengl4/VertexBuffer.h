#pragma once

#include <span>

#include <glad/gl.h>

#include "../IVertexBuffer.h"
#include "../enums.h"

namespace yage::gl::opengl4
{
    class Context;

    class VertexBuffer final : public IVertexBuffer
    {
    public:
        VertexBuffer(Context* context, std::span<const std::byte> data, ResourceUsage usage);

        ~VertexBuffer() override;

        VertexBuffer(const VertexBuffer& other) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept;

        VertexBuffer& operator=(const VertexBuffer& other) = delete;

        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        [[nodiscard]] GLuint gl_handle() const;

    private:
        Context* m_context;

        GLuint m_vbo_handle = 0;

        void clear();
    };
}
