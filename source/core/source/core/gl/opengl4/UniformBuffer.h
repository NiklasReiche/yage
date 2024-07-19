#pragma once

#include <glad/gl.h>

#include "../IUniformBuffer.h"
#include "../enums.h"

namespace yage::gl::opengl4
{
    class Context;

    class UniformBuffer final : public IUniformBuffer
    {
    public:
        UniformBuffer(Context* context, std::size_t buffer_size, ResourceUsage usage);

        ~UniformBuffer() override;

        UniformBuffer(const UniformBuffer& other) = delete;

        UniformBuffer(UniformBuffer&& other) noexcept;

        UniformBuffer& operator=(const UniformBuffer& other) = delete;

        UniformBuffer& operator=(UniformBuffer&& other) noexcept;

        void update_sub_data(std::size_t byte_offset, std::size_t byte_size, const void* data) override;

    private:
        Context* m_context;

        GLuint m_ubo_handle = 0;

        void clear();
    };
}
