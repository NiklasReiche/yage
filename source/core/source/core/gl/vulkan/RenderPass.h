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

        RenderPass(const RenderPass& other) = delete;

        RenderPass(RenderPass&& other) noexcept
        {
            m_instance = std::move(other.m_instance);
            m_vk_handle = other.m_vk_handle;

            other.m_vk_handle = VK_NULL_HANDLE;
        }

        RenderPass& operator=(const RenderPass& other) = delete;

        RenderPass& operator=(RenderPass&& other) noexcept
        {
            m_instance = std::move(other.m_instance);
            m_vk_handle = other.m_vk_handle;

            other.m_vk_handle = VK_NULL_HANDLE;

            return *this;
        }

        [[nodiscard]] VkRenderPass vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkRenderPass m_vk_handle{};
    };
}
