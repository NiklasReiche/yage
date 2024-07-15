#include "Renderer.h"
#include "Drawable.h"
#include "IndexBuffer.h"
#include "Instance.h"
#include "VertexBuffer.h"
#include "enums.h"

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

    void Renderer::begin_render_pass()
    {
        const auto instance = m_instance.lock();
        auto& swap_chain = instance->swap_chain();

        instance->transition_from_undefined_to_color_attachment_optimal(m_command_buffer, swap_chain.present_image());

        VkRenderingInfo rendering_info{};
        rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        rendering_info.flags = 0;
        rendering_info.renderArea.offset = {0, 0};
        rendering_info.renderArea.extent = swap_chain.extent();
        rendering_info.layerCount = 1;
        rendering_info.viewMask = 0;
        rendering_info.colorAttachmentCount = 1;
        rendering_info.pColorAttachments = &swap_chain.color_attachment();
        rendering_info.pDepthAttachment = &swap_chain.depth_attachment();

        vkCmdBeginRendering(m_command_buffer, &rendering_info);

        m_render_to_swap_chain = true;
    }

    void Renderer::begin_render_pass(const RenderTarget& render_target)
    {
        math::Vec2ui render_target_size = render_target.extent();
        const std::vector<VkRenderingAttachmentInfo>& color_attachment_infos = render_target.color_attachment_infos();
        const std::optional<VkRenderingAttachmentInfo>& depth_attachment_info = render_target.depth_attachment_info();

        VkRenderingInfo rendering_info{};
        rendering_info.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        rendering_info.flags = 0;
        rendering_info.renderArea = {0, 0, render_target_size.x(), render_target_size.y()};
        rendering_info.layerCount = 1;
        rendering_info.viewMask = 0;
        rendering_info.colorAttachmentCount = color_attachment_infos.size();
        rendering_info.pColorAttachments = color_attachment_infos.data();
        if (depth_attachment_info.has_value()) {
            rendering_info.pDepthAttachment = &depth_attachment_info.value();
        }
        // TODO: stencil attachment

        vkCmdBeginRendering(m_command_buffer, &rendering_info);

        m_render_to_swap_chain = false;
    }

    void Renderer::bind_pipeline(const IPipeline& pipeline, const IDescriptorSet& descriptor_set)
    {
        const auto& descriptor_set_impl = static_cast<const DescriptorSet&>(descriptor_set);
        const auto& pipeline_impl = static_cast<const Pipeline&>(pipeline);

        vkCmdBindDescriptorSets(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_impl.vk_layout(), 0, 1,
                                &descriptor_set_impl.vk_handle(), 0, nullptr);

        vkCmdBindPipeline(m_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_impl.vk_handle());
    }

    void Renderer::draw(const IDrawable2& drawable)
    {
        const auto& descriptor = static_cast<const Drawable&>(drawable).descriptor();

        const VkBuffer vertex_buffers[] = {descriptor.vertex_buffer.get<VertexBuffer>().vk_handle()};
        const VkDeviceSize vertex_buffer_offsets[] = {descriptor.vertex_buffer_offset};
        // TODO: bindings
        vkCmdBindVertexBuffers(m_command_buffer, 0, 1, vertex_buffers, vertex_buffer_offsets);

        vkCmdBindIndexBuffer(m_command_buffer, descriptor.index_buffer.get<IndexBuffer>().vk_handle(),
                             descriptor.index_buffer_offset, convert(descriptor.index_data_info.data_type));

        vkCmdDrawIndexed(m_command_buffer, descriptor.index_data_info.index_count, 1, 0, 0, 0);
    }

    void Renderer::end_render_pass()
    {
        vkCmdEndRendering(m_command_buffer);

        if (m_render_to_swap_chain) {
            const auto instance = m_instance.lock();
            auto& swap_chain = instance->swap_chain();
            instance->transition_from_color_attachment_optimal_to_present_src(m_command_buffer,
                                                                              swap_chain.present_image());
        }
    }

    void Renderer::end_command_buffer()
    {
        if (vkEndCommandBuffer(m_command_buffer) != VK_SUCCESS) {
            throw std::runtime_error("Vulkan: failed to record command buffer!");
        }

        m_command_buffer = nullptr;
    }
}
