#pragma once

#include <span>

#include <glad/gl.h>

#include "../IIndexBuffer.h"
#include "../enums.h"

namespace yage::gl::opengl4
{
    class Context;

    class IndexBuffer final : public IIndexBuffer
    {
    public:
        IndexBuffer(Context* context, std::span<const std::byte> indices, ResourceUsage usage);

        ~IndexBuffer() override;

        IndexBuffer(const IndexBuffer& other) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept;

        IndexBuffer& operator=(const IndexBuffer& other) = delete;

        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        [[nodiscard]] GLuint gl_handle() const;

    private:
        Context* m_context;

        GLuint m_ebo_handle = 0;

        void clear();
    };
}
