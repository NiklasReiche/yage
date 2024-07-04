#pragma once

#include "../IIndexBufferCreator.h"

namespace yage::gl::vulkan
{
    class Instance;

    class IndexBufferCreator final : IIndexBufferCreator
    {
    public:
        explicit IndexBufferCreator(std::weak_ptr<Instance> instance);

        [[nodiscard]] Handle<IIndexBuffer> create(IndexDataInfo data_info,
                                                  std::span<const std::byte> data) const override;

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
