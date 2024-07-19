#pragma once

#include "../IUniformBufferCreator.h"

namespace yage::gl::opengl4
{
    class Context;

    class UniformBufferCreator final : public IUniformBufferCreator
    {
    public:
        explicit UniformBufferCreator(Context* context);

        [[nodiscard]] UniformBufferHandle create(std::size_t byte_size, ResourceUsage usage) const override;

    private:
        Context* m_context; // TODO
    };
}
