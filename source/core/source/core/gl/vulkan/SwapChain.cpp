#include "SwapChain.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    SwapChain::SwapChain(Instance* instance, const VkSwapchainCreateInfoKHR& create_info, const VkFormat depth_format,
                         const VkSampleCountFlagBits msaa_samples)
        : m_instance(instance),
          m_vk_device(instance->device())
    {
        if (vkCreateSwapchainKHR(m_vk_device, &create_info, nullptr, &m_swap_chain_khr) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(m_vk_device, m_swap_chain_khr, &m_max_image_index, nullptr); // TODO: error handling
        m_resolve_images.resize(m_max_image_index);
        vkGetSwapchainImagesKHR(m_vk_device, m_swap_chain_khr, &m_max_image_index,
                                m_resolve_images.data()); // TODO: error handling
        // swap chain images are transitioned during rendering (from UNDEFINED to PRESENT_SRC)

        m_image_format = create_info.imageFormat;
        m_extent = create_info.imageExtent;
        m_samples = msaa_samples;
        m_depth_image_format = depth_format;

        create_image_views();
        create_color_resource();
        create_depth_resource();
        create_attachment_infos();
    }

    SwapChain::~SwapChain()
    {
        clear();
    }

    SwapChain::SwapChain(SwapChain&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_swap_chain_khr(other.m_swap_chain_khr),
          m_image_format(other.m_image_format),
          m_extent(other.m_extent),
          m_resolve_images(std::move(other.m_resolve_images)),
          m_resolve_image_views(std::move(other.m_resolve_image_views)),
          m_resolve_attachment_infos(std::move(other.m_resolve_attachment_infos)),
          m_depth_image_format(other.m_depth_image_format),
          m_depth_image(other.m_depth_image),
          m_depth_image_memory(other.m_depth_image_memory),
          m_depth_image_view(other.m_depth_image_view),
          m_depth_attachment_info(other.m_depth_attachment_info),
          m_samples(other.m_samples),
          m_color_image(other.m_color_image),
          m_color_image_memory(other.m_color_image_memory),
          m_color_image_view(other.m_color_image_view),
          m_max_image_index(other.m_max_image_index),
          m_current_image_index(other.m_current_image_index)
    {
        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;

        other.m_swap_chain_khr = VK_NULL_HANDLE;
        other.m_image_format = VK_FORMAT_UNDEFINED;
        other.m_extent = VkExtent2D{};

        other.m_depth_image_format = VK_FORMAT_UNDEFINED;
        other.m_depth_image_view = VK_NULL_HANDLE;
        other.m_depth_image = VK_NULL_HANDLE;
        other.m_depth_image_memory = VK_NULL_HANDLE;

        other.m_samples = VK_SAMPLE_COUNT_1_BIT;
        other.m_color_image = VK_NULL_HANDLE;
        other.m_color_image_memory = VK_NULL_HANDLE;
        other.m_color_image_view = VK_NULL_HANDLE;
    }

    SwapChain& SwapChain::operator=(SwapChain&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;

        m_swap_chain_khr = other.m_swap_chain_khr;
        m_image_format = other.m_image_format;
        m_extent = other.m_extent;
        m_resolve_images = std::move(other.m_resolve_images);
        m_resolve_image_views = std::move(other.m_resolve_image_views);
        m_resolve_attachment_infos = std::move(other.m_resolve_attachment_infos);
        m_depth_image_format = other.m_depth_image_format;
        m_depth_image = other.m_depth_image;
        m_depth_image_memory = other.m_depth_image_memory;
        m_depth_image_view = other.m_depth_image_view;
        m_depth_attachment_info = other.m_depth_attachment_info;
        m_max_image_index = other.m_max_image_index;
        m_current_image_index = other.m_current_image_index;
        m_samples = other.m_samples;
        m_color_image = other.m_color_image;
        m_color_image_memory = other.m_color_image_memory;
        m_color_image_view = other.m_color_image_view;

        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;

        other.m_swap_chain_khr = VK_NULL_HANDLE;
        other.m_image_format = VK_FORMAT_UNDEFINED;
        other.m_extent = VkExtent2D{};

        other.m_depth_image_format = VK_FORMAT_UNDEFINED;
        other.m_depth_image_view = VK_NULL_HANDLE;
        other.m_depth_image = VK_NULL_HANDLE;
        other.m_depth_image_memory = VK_NULL_HANDLE;

        other.m_samples = VK_SAMPLE_COUNT_1_BIT;
        other.m_color_image = VK_NULL_HANDLE;
        other.m_color_image_memory = VK_NULL_HANDLE;
        other.m_color_image_view = VK_NULL_HANDLE;

        return *this;
    }

    VkResult SwapChain::acquire_next_image(const VkSemaphore semaphore)
    {
        return vkAcquireNextImageKHR(m_vk_device, m_swap_chain_khr, UINT64_MAX, semaphore, VK_NULL_HANDLE,
                                     &m_current_image_index);
    }

    void SwapChain::fill_present_info(VkPresentInfoKHR& present_info)
    {
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_swap_chain_khr;
        present_info.pImageIndices = &m_current_image_index;
        present_info.pResults = nullptr; // Optional
    }

    void SwapChain::clear()
    {
        m_resolve_attachment_infos.clear();

        if (m_depth_image_view != VK_NULL_HANDLE) {
            vkDestroyImageView(m_vk_device, m_depth_image_view, nullptr);
            m_depth_image_view = VK_NULL_HANDLE;
        }
        if (m_depth_image != VK_NULL_HANDLE) {
            vkDestroyImage(m_vk_device, m_depth_image, nullptr);
            m_depth_image = VK_NULL_HANDLE;
        }
        if (m_depth_image_memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_vk_device, m_depth_image_memory, nullptr);
            m_depth_image_memory = VK_NULL_HANDLE;
        }
        m_depth_attachment_info = VkRenderingAttachmentInfo{};

        if (m_color_image_view != VK_NULL_HANDLE) {
            vkDestroyImageView(m_vk_device, m_color_image_view, nullptr);
            m_color_image_view = VK_NULL_HANDLE;
        }
        if (m_color_image != VK_NULL_HANDLE) {
            vkDestroyImage(m_vk_device, m_color_image, nullptr);
            m_color_image = VK_NULL_HANDLE;
        }
        if (m_color_image_memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_vk_device, m_color_image_memory, nullptr);
            m_color_image_memory = VK_NULL_HANDLE;
        }

        for (const auto& m_image_view: m_resolve_image_views) {
            vkDestroyImageView(m_vk_device, m_image_view, nullptr);
        }
        m_resolve_image_views.clear();

        // swap chain images are destroyed implicitly
        if (m_swap_chain_khr != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(m_vk_device, m_swap_chain_khr, nullptr);
            m_swap_chain_khr = VK_NULL_HANDLE;
        }
    }

    VkExtent2D SwapChain::extent() const
    {
        return m_extent;
    }

    VkSampleCountFlagBits SwapChain::samples() const
    {
        return m_samples;
    }

    const VkFormat& SwapChain::color_format() const
    {
        return m_image_format;
    }

    const VkRenderingAttachmentInfo& SwapChain::color_attachment() const
    {
        return m_resolve_attachment_infos[m_current_image_index];
    }

    const VkFormat& SwapChain::depth_format() const
    {
        return m_depth_image_format;
    }

    const VkRenderingAttachmentInfo& SwapChain::depth_attachment() const
    {
        return m_depth_attachment_info;
    }

    VkImage& SwapChain::present_image()
    {
        return m_resolve_images[m_current_image_index];
    }

    VkSwapchainKHR& SwapChain::swapchain_khr()
    {
        return m_swap_chain_khr;
    }

    void SwapChain::create_image_views()
    {
        m_resolve_image_views.resize(m_resolve_images.size());
        for (std::size_t i = 0; i < m_resolve_images.size(); i++) {
            m_resolve_image_views[i] =
                    m_instance->create_image_view(m_resolve_images[i], m_image_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }

    void SwapChain::create_color_resource()
    {
        m_instance->create_image(m_extent.width, m_extent.height, 1, m_samples, m_image_format, VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                 m_color_image, m_color_image_memory);

        m_color_image_view = m_instance->create_image_view(m_color_image, m_image_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

        m_instance->transition_from_undefined_to_color_attachment_optimal(m_color_image);
    }

    void SwapChain::create_depth_resource()
    {
        m_instance->create_image(m_extent.width, m_extent.height, 1, m_samples, m_depth_image_format,
                                 VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depth_image, m_depth_image_memory);
        m_depth_image_view =
                m_instance->create_image_view(m_depth_image, m_depth_image_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

        m_instance->transition_from_undefined_to_depth_stencil_attachment_optimal(m_depth_image);
    }

    void SwapChain::create_attachment_infos()
    {
        m_resolve_attachment_infos.resize(m_resolve_image_views.size());
        for (std::size_t i = 0; i < m_resolve_image_views.size(); i++) {
            VkRenderingAttachmentInfo attachment_info{};
            attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;

            attachment_info.imageView = m_color_image_view;
            attachment_info.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            attachment_info.resolveMode = VK_RESOLVE_MODE_AVERAGE_BIT;
            attachment_info.resolveImageView = m_resolve_image_views[i];
            attachment_info.resolveImageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachment_info.clearValue = {0.0f, 0.0f, 0.0f, 1.0f};

            m_resolve_attachment_infos[i] = attachment_info;
        }

        m_depth_attachment_info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;

        m_depth_attachment_info.imageView = m_depth_image_view;
        m_depth_attachment_info.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        m_depth_attachment_info.resolveMode = VK_RESOLVE_MODE_NONE;
        m_depth_attachment_info.resolveImageView = VK_NULL_HANDLE;
        m_depth_attachment_info.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        m_depth_attachment_info.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        m_depth_attachment_info.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        m_depth_attachment_info.clearValue = {1.0f};
    }
}
