#include "Renderer.h"

namespace yage::gl::vulkan
{
    Renderer::Renderer(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    void Renderer::begin_command_buffer()
    {
        m_command_buffer = m_instance.lock()->command_buffer_for_frame();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(m_command_buffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to begin recording command buffer!");
        }
    }

    void Renderer::begin_render_pass(const FrameBuffer& frame_buffer)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = frame_buffer.render_pass()->vk_handle();
        renderPassInfo.framebuffer = frame_buffer.vk_handle();
        renderPassInfo.renderArea.offset = {0, 0};
        const math::Vec2ui extent = frame_buffer.extent();
        renderPassInfo.renderArea.extent = {extent.x(), extent.y()};
        constexpr VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(m_command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void Renderer::bind_pipeline(const Pipeline& pipeline)
    {
        vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.vk_handle());
    }

    void Renderer::draw()
    {
        vkCmdDraw(m_command_buffer, 3, 1, 0, 0);
    }

    void Renderer::end_render_pass()
    {
        vkCmdEndRenderPass(m_command_buffer);
    }

    void Renderer::end_command_buffer()
    {
        if (vkEndCommandBuffer(m_command_buffer) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to record command buffer!");
        }

        m_command_buffer = nullptr;
    }
}
