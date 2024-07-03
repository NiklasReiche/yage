#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "../Handle.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderPass
    {
    public:
        explicit RenderPass(std::weak_ptr<Instance> instance, const VkRenderPassCreateInfo& create_info);

        ~RenderPass();

        RenderPass(const RenderPass& other) = delete;

        RenderPass(RenderPass&& other) noexcept;

        RenderPass& operator=(const RenderPass& other) = delete;

        RenderPass& operator=(RenderPass&& other) noexcept;

        [[nodiscard]] VkRenderPass vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkDevice m_vk_device;
        VkRenderPass m_vk_handle{};
    };

    using RenderPassHandle = Handle<RenderPass>;
}
