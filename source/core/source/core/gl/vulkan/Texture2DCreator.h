#pragma once

#include "../ITexture2DCreator.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Texture2DCreator final : public ITexture2DCreator
    {
    public:
        explicit Texture2DCreator(std::weak_ptr<Instance> instance);

        [[nodiscard]] Texture2DHandle create(const TextureSampler& sampler, const PixelTransferInfo& data_info,
                                             std::span<const std::byte> data, ResourceUsage usage) const override;

        [[nodiscard]] Texture2DHandle create_multisampled(MSAASamples samples, const PixelTransferInfo& data_info,
                                                          ResourceUsage usage) const override;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
    };
}
