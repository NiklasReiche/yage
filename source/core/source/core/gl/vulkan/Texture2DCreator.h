#pragma once

#include "../ITexture2DCreator.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Texture2DCreator final : public ITexture2DCreator
    {
    public:
        explicit Texture2DCreator(std::weak_ptr<Instance> instance);

        [[nodiscard]] Texture2DHandle create(const PixelTransferInfo& data_info,
                                             std::span<const std::byte> data) const override;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
    };
}
