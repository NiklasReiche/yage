#include "FrameBuffer.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    FrameBuffer::FrameBuffer(std::weak_ptr<Instance> instance, std::shared_ptr<RenderPass> render_pass,
                             VkFramebufferCreateInfo& create_info)
        : m_instance(std::move(instance)),
          m_render_pass(std::move(render_pass)),
          m_extent({create_info.width, create_info.height})
    {
        create_info.renderPass = m_render_pass->vk_handle();
        if (vkCreateFramebuffer(m_instance.lock()->device(), &create_info, nullptr, &m_vk_handle) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to create frame buffer!");
        }
    }

    FrameBuffer::~FrameBuffer()
    {
        vkDestroyFramebuffer(m_instance.lock()->device(), m_vk_handle, nullptr);
    }

    VkFramebuffer FrameBuffer::vk_handle() const
    {
        return m_vk_handle;
    }

    std::shared_ptr<RenderPass> FrameBuffer::render_pass() const
    {
        return m_render_pass;
    }

    math::Vec2ui FrameBuffer::extent() const
    {
        return m_extent;
    }
}
