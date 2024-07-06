#include "FrameBufferCreator.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    FrameBufferCreator::FrameBufferCreator(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<IFrameBuffer> FrameBufferCreator::create(std::shared_ptr<Handle<RenderPass>> render_pass,
                                                    const std::span<std::shared_ptr<ImageViewHandle>> attachements,
                                                    const std::uint32_t width, const std::uint32_t height,
                                                    const ResourceUsage usage)
    {

        VkFramebufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.width = width;
        create_info.height = height;
        create_info.layers = 1;

        FrameCounter frame_counter{};
        switch (usage) {
            case ResourceUsage::STATIC:  frame_counter = m_instance.lock()->static_counter(); break;
            case ResourceUsage::DYNAMIC: frame_counter = m_instance.lock()->frames_in_flight_counter(); break;

            default: throw std::invalid_argument("unkown ResourceUsage value");
        }

        return m_instance.lock()->store_frame_buffers()->create(m_instance, std::move(render_pass), attachements,
                                                                create_info, frame_counter);
    }

    FrameBufferHandle
    FrameBufferCreator::create_swap_chain(std::shared_ptr<RenderPassHandle> render_pass,
                                          const std::span<std::shared_ptr<ImageViewHandle>> attachements,
                                          const std::uint32_t width, const std::uint32_t height)
    {
        VkFramebufferCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        create_info.width = width;
        create_info.height = height;
        create_info.layers = 1;

        return m_instance.lock()->store_frame_buffers()->create(m_instance, std::move(render_pass), attachements,
                                                                create_info, m_instance.lock()->swap_chain_counter());
    }
}
