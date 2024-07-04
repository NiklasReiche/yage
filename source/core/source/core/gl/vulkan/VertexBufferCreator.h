#pragma once

#include "../IVertexBufferCreator.h"

namespace yage::gl::vulkan
{
    class Instance;

    class VertexBufferCreator final : public IVertexBufferCreator
    {
    public:
        explicit VertexBufferCreator(std::weak_ptr<Instance> instance);

        [[nodiscard]] Handle<IVertexBuffer> create(const VertexDataInfo& data_info,
                                                   std::span<const std::byte> data) const override;

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
