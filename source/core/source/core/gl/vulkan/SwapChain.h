#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class SwapChain
    {
    public:
        SwapChain() = default;

        SwapChain(Instance* instance, const VkSwapchainCreateInfoKHR& create_info, VkFormat depth_format);

        ~SwapChain();

        SwapChain(const SwapChain& other) = delete;

        SwapChain(SwapChain&& other) noexcept;

        SwapChain& operator=(SwapChain& other) = delete;

        SwapChain& operator=(SwapChain&& other) noexcept;

        VkResult acquire_next_image(VkSemaphore semaphore);

        void fill_present_info(VkPresentInfoKHR& present_info);

        void clear();

        [[nodiscard]] const RenderPassHandle& render_pass() const; // TODO: return handle to const

        [[nodiscard]] VkFramebuffer current_frame_buffer() const;

        [[nodiscard]] VkExtent2D extent() const;

    private:
        Instance* m_instance = nullptr;
        VkDevice m_vk_device = VK_NULL_HANDLE;

        VkSwapchainKHR m_swap_chain_khr = VK_NULL_HANDLE;
        VkFormat m_image_format{};
        VkExtent2D m_extent{};

        RenderPassHandle m_render_pass;

        std::vector<VkImage> m_images;
        std::vector<VkImageView> m_image_views;
        std::vector<VkFramebuffer> m_frame_buffers;

        VkFormat m_depth_image_format{};
        VkImage m_depth_image = VK_NULL_HANDLE;
        VkDeviceMemory m_depth_image_memory = VK_NULL_HANDLE;
        VkImageView m_depth_image_view = VK_NULL_HANDLE;

        unsigned int m_max_image_index = 0;
        unsigned int m_current_image_index = 0;

        void create_image_views();

        void create_depth_resource();

        void create_render_pass();

        void create_frame_buffers();
    };
}
