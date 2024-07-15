#pragma once

#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace yage::gl::vulkan
{
    class Instance;

    class SwapChain
    {
    public:
        SwapChain() = default;

        SwapChain(Instance* instance, const VkSwapchainCreateInfoKHR& create_info, VkFormat depth_format,
                  VkSampleCountFlagBits msaa_samples);

        ~SwapChain();

        SwapChain(const SwapChain& other) = delete;

        SwapChain(SwapChain&& other) noexcept;

        SwapChain& operator=(SwapChain& other) = delete;

        SwapChain& operator=(SwapChain&& other) noexcept;

        VkResult acquire_next_image(VkSemaphore semaphore);

        void fill_present_info(VkPresentInfoKHR& present_info);

        void clear();

        [[nodiscard]] VkExtent2D extent() const;

        [[nodiscard]] VkSampleCountFlagBits samples() const;

        [[nodiscard]] const VkFormat& color_format() const;

        [[nodiscard]] const VkRenderingAttachmentInfo& color_attachment() const;

        [[nodiscard]] const VkFormat& depth_format() const;

        [[nodiscard]] const VkRenderingAttachmentInfo& depth_attachment() const;

        [[nodiscard]] VkImage& present_image();

    private:
        Instance* m_instance = nullptr;
        VkDevice m_vk_device = VK_NULL_HANDLE;

        VkSwapchainKHR m_swap_chain_khr = VK_NULL_HANDLE;
        VkFormat m_image_format = VK_FORMAT_UNDEFINED;
        VkExtent2D m_extent{};

        std::vector<VkImage> m_resolve_images;
        std::vector<VkImageView> m_resolve_image_views;
        std::vector<VkRenderingAttachmentInfo> m_resolve_attachment_infos;

        // only one depth image, since its contents don't need to be preserved after rendering the frame
        VkFormat m_depth_image_format = VK_FORMAT_UNDEFINED;
        VkImage m_depth_image = VK_NULL_HANDLE;
        VkDeviceMemory m_depth_image_memory = VK_NULL_HANDLE;
        VkImageView m_depth_image_view = VK_NULL_HANDLE;
        VkRenderingAttachmentInfo m_depth_attachment_info{};

        // only one color image, since its contents don't need to be preserved after rendering the frame and resolving
        VkSampleCountFlagBits m_samples = VK_SAMPLE_COUNT_1_BIT;
        VkImage m_color_image = VK_NULL_HANDLE;
        VkDeviceMemory m_color_image_memory = VK_NULL_HANDLE;
        VkImageView m_color_image_view = VK_NULL_HANDLE;

        unsigned int m_max_image_index = 0;
        unsigned int m_current_image_index = 0;

        void create_image_views();

        void create_color_resource();

        void create_depth_resource();

        void create_attachment_infos();
    };
}
