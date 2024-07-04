#pragma once

#include "../IVertexBufferFactory.h"

namespace yage::gl::vulkan
{
    class Instance;

    class VertexBufferFactory final : public IVertexBufferFactory
    {
    public:
        explicit VertexBufferFactory(std::weak_ptr<Instance> instance);

        Handle<IVertexBuffer> create(const VertexDataInfo& data_info,
                                     std::span<const std::byte> data) override;

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
