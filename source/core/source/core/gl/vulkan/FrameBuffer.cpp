#include "FrameBuffer.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    FrameBuffer::FrameBuffer(std::weak_ptr<Instance> instance, std::shared_ptr<RenderPassHandle> render_pass,
                             VkFramebufferCreateInfo& create_info)
        : m_instance(std::move(instance)),
          m_vk_device(m_instance.lock()->device()),
          m_render_pass(std::move(render_pass)),
          m_extent({create_info.width, create_info.height})
    {
        create_info.renderPass = m_render_pass->get<RenderPass>().vk_handle();
        if (vkCreateFramebuffer(m_vk_device, &create_info, nullptr, &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create frame buffer!");
        }
    }

    FrameBuffer::~FrameBuffer()
    {
        if (m_vk_handle != VK_NULL_HANDLE) {
            vkDestroyFramebuffer(m_vk_device, m_vk_handle, nullptr);
        }
    }

    FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
        : m_instance(std::move(other.m_instance)),
          m_vk_device(other.m_vk_device),
          m_render_pass(std::move(other.m_render_pass)),
          m_vk_handle(other.m_vk_handle),
          m_extent(other.m_extent)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
        other.m_extent = {0, 0};
    }

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = std::move(other.m_instance);
        m_vk_device = other.m_vk_device;
        m_render_pass = std::move(other.m_render_pass);
        m_vk_handle = other.m_vk_handle;
        m_extent = other.m_extent;

        other.m_vk_device = VK_NULL_HANDLE;
        other.m_vk_handle = VK_NULL_HANDLE;
        other.m_extent = {0, 0};

        return *this;
    }

    VkFramebuffer FrameBuffer::vk_handle() const
    {
        return m_vk_handle;
    }

    std::shared_ptr<RenderPassHandle> FrameBuffer::render_pass() const
    {
        return m_render_pass;
    }

    math::Vec2ui FrameBuffer::extent() const
    {
        return m_extent;
    }
}
