#pragma once

#include "../IVertexBufferFactory.h"

namespace yage::gl::vulkan
{
    class Instance;

    class VertexBufferFactory final : public IVertexBufferFactory
    {
    public:
        explicit VertexBufferFactory(std::weak_ptr<Instance> instance);

        Handle<IVertexBuffer> create_test(const std::span<const std::byte>& data, std::size_t vertex_count) override;

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
