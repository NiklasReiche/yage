#pragma once

#include <vulkan/vulkan.h>

#include "../IRenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderPass final : public IRenderPass
    {
    public:
        explicit RenderPass(Instance* instance, const VkRenderPassCreateInfo& create_info, VkSampleCountFlagBits samples);

        ~RenderPass() override;

        RenderPass(const RenderPass& other) = delete;

        RenderPass(RenderPass&& other) noexcept;

        RenderPass& operator=(const RenderPass& other) = delete;

        RenderPass& operator=(RenderPass&& other) noexcept;

        [[nodiscard]] VkRenderPass vk_handle() const;

        VkSampleCountFlagBits samples();

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        VkRenderPass m_vk_handle{};

        VkSampleCountFlagBits m_msaa_samples;
    };
}
