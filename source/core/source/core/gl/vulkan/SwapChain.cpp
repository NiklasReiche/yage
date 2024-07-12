#include "SwapChain.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    SwapChain::SwapChain(Instance* instance, const VkSwapchainCreateInfoKHR& create_info, const VkFormat depth_format)
        : m_instance(instance),
          m_vk_device(instance->device())
    {
        if (vkCreateSwapchainKHR(m_vk_device, &create_info, nullptr, &m_swap_chain_khr) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(m_vk_device, m_swap_chain_khr, &m_max_image_index, nullptr); // TODO: error handling
        m_images.resize(m_max_image_index);
        vkGetSwapchainImagesKHR(m_vk_device, m_swap_chain_khr, &m_max_image_index,
                                m_images.data()); // TODO: error handling

        m_image_format = create_info.imageFormat;
        m_extent = create_info.imageExtent;
        m_depth_image_format = depth_format;

        create_image_views();
        create_depth_resource();
        create_render_pass();
        create_frame_buffers();
    }

    SwapChain::~SwapChain()
    {
        if (m_swap_chain_khr != VK_NULL_HANDLE) {
            clear();
        }
    }

    SwapChain::SwapChain(SwapChain&& other) noexcept
        : m_instance(other.m_instance),
          m_vk_device(other.m_vk_device),
          m_swap_chain_khr(other.m_swap_chain_khr),
          m_image_format(other.m_image_format),
          m_extent(other.m_extent),
          m_render_pass(std::move(other.m_render_pass)),
          m_images(std::move(other.m_images)),
          m_image_views(std::move(other.m_image_views)),
          m_frame_buffers(std::move(other.m_frame_buffers)),
          m_depth_image(other.m_depth_image),
          m_depth_image_memory(other.m_depth_image_memory),
          m_depth_image_view(other.m_depth_image_view),
          m_max_image_index(other.m_max_image_index),
          m_current_image_index(other.m_current_image_index)
    {
        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_swap_chain_khr = VK_NULL_HANDLE;
        other.m_depth_image_view = VK_NULL_HANDLE;
        other.m_depth_image = VK_NULL_HANDLE;
        other.m_depth_image_memory = VK_NULL_HANDLE;
    }

    SwapChain& SwapChain::operator=(SwapChain&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_instance = other.m_instance;
        m_vk_device = other.m_vk_device;
        m_swap_chain_khr = other.m_swap_chain_khr;
        m_image_format = other.m_image_format;
        m_extent = other.m_extent;
        m_render_pass = std::move(other.m_render_pass);
        m_images = std::move(other.m_images);
        m_image_views = std::move(other.m_image_views);
        m_frame_buffers = std::move(other.m_frame_buffers);
        m_depth_image = other.m_depth_image;
        m_depth_image_memory = other.m_depth_image_memory;
        m_depth_image_view = other.m_depth_image_view;
        m_max_image_index = other.m_max_image_index;
        m_current_image_index = other.m_current_image_index;

        other.m_instance = nullptr;
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_swap_chain_khr = VK_NULL_HANDLE;
        other.m_depth_image_view = VK_NULL_HANDLE;
        other.m_depth_image = VK_NULL_HANDLE;
        other.m_depth_image_memory = VK_NULL_HANDLE;

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
        for (const auto& m_frame_buffer: m_frame_buffers) {
            vkDestroyFramebuffer(m_vk_device, m_frame_buffer, nullptr);
        }
        m_frame_buffers.clear();

        m_render_pass.reset();

        if (m_depth_image_view != VK_NULL_HANDLE) {
            vkDestroyImageView(m_vk_device, m_depth_image_view, nullptr);
        }
        if (m_depth_image != VK_NULL_HANDLE) {
            vkDestroyImage(m_vk_device, m_depth_image, nullptr);
        }
        if (m_depth_image_memory != VK_NULL_HANDLE) {
            vkFreeMemory(m_vk_device, m_depth_image_memory, nullptr);
        }

        for (const auto& m_image_view: m_image_views) {
            vkDestroyImageView(m_vk_device, m_image_view, nullptr);
        }
        m_image_views.clear();

        // swap chain images are destroyed implicitly
        vkDestroySwapchainKHR(m_vk_device, m_swap_chain_khr, nullptr);
        m_swap_chain_khr = VK_NULL_HANDLE;
    }

    const RenderPassHandle& SwapChain::render_pass() const
    {
        return m_render_pass;
    }

    VkFramebuffer SwapChain::current_frame_buffer() const
    {
        return m_frame_buffers[m_current_image_index];
    }

    VkExtent2D SwapChain::extent() const
    {
        return m_extent;
    }

    void SwapChain::create_image_views()
    {
        m_image_views.resize(m_images.size());
        for (std::size_t i = 0; i < m_images.size(); i++) {
            m_image_views[i] = m_instance->create_image_view(m_images[i], m_image_format, VK_IMAGE_ASPECT_COLOR_BIT);
        }
    }

    void SwapChain::create_depth_resource()
    {
        m_instance->create_image(m_extent.width, m_extent.height, m_depth_image_format, VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depth_image,
                    m_depth_image_memory);
        m_depth_image_view = m_instance->create_image_view(m_depth_image, m_depth_image_format, VK_IMAGE_ASPECT_DEPTH_BIT);

        // TODO: transition?
    }

    void SwapChain::create_render_pass()
    {
        VkAttachmentDescription color_attachment{};
        color_attachment.format = m_image_format;
        color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        VkAttachmentReference color_attachment_ref{};
        color_attachment_ref.attachment = 0;
        color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depth_attachment{};
        depth_attachment.format = m_depth_image_format;
        depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        VkAttachmentReference depth_attachment_ref{};
        depth_attachment_ref.attachment = 1;
        depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_attachment_ref;
        subpass.pDepthStencilAttachment = &depth_attachment_ref;

        const std::array attachments = {color_attachment, depth_attachment};
        VkRenderPassCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        create_info.attachmentCount = static_cast<std::uint32_t>(attachments.size());
        create_info.pAttachments = &attachments[0];
        create_info.subpassCount = 1;
        create_info.pSubpasses = &subpass;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        create_info.dependencyCount = 1;
        create_info.pDependencies = &dependency;

        m_render_pass = m_instance->store_render_passes().create(m_instance, create_info);
    }

    void SwapChain::create_frame_buffers()
    {
        m_frame_buffers.resize(m_image_views.size());
        for (std::size_t i = 0; i < m_image_views.size(); i++) {
            const std::array attachments = {m_image_views[i], m_depth_image_view};

            VkFramebufferCreateInfo create_info{};
            create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            create_info.renderPass = m_render_pass.get<RenderPass>().vk_handle();
            create_info.attachmentCount = static_cast<std::uint32_t>(attachments.size());
            create_info.pAttachments = &attachments[0];
            create_info.width = m_extent.width;
            create_info.height = m_extent.height;
            create_info.layers = 1;

            if (vkCreateFramebuffer(m_vk_device, &create_info, nullptr, &m_frame_buffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create swap chain framebuffers!");
            }
        }
    }
}
