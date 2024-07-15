#pragma once

#include "../IIndexBufferCreator.h"

namespace yage::gl::vulkan
{
    class Instance;

    class IndexBufferCreator final : public IIndexBufferCreator
    {
    public:
        explicit IndexBufferCreator(std::weak_ptr<Instance> instance);

        [[nodiscard]] IndexBufferHandle create(IndexDataInfo data_info, std::span<const std::byte> data,
                                               ResourceUsage usage) const override;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
    };
}
