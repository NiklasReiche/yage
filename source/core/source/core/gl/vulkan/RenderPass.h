#pragma once

#include <memory>

#include <vulkan/vulkan.h>

namespace yage::gl::vulkan
{
    class Instance;

    class RenderPass
    {
    public:
        explicit RenderPass(std::weak_ptr<Instance> instance, const VkRenderPassCreateInfo& create_info);

        ~RenderPass();

        [[nodiscard]] VkRenderPass vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkRenderPass m_vk_handle{};
    };
}
