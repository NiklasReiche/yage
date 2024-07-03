#include "FrameBufferFactory.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    FrameBufferFactory::FrameBufferFactory(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    Handle<IFrameBuffer> FrameBufferFactory::create_frame_buffer(std::shared_ptr<Handle<RenderPass>> render_pass,
                                                                std::span<ImageViewHandle*> attachements,
                                                                const unsigned int width, const unsigned int height)
    {
        std::vector<VkImageView> vk_attachements;
        vk_attachements.reserve(attachements.size());
        std::ranges::transform(attachements, std::back_inserter(vk_attachements),
                               [](const ImageViewHandle* x) { return x->get<ImageView>().vk_handle(); });

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = render_pass->get<RenderPass>().vk_handle();
        framebufferInfo.attachmentCount = vk_attachements.size();
        framebufferInfo.pAttachments = vk_attachements.data();
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = 1;

        return m_instance.lock()->store_frame_buffers()->create(m_instance, std::move(render_pass), framebufferInfo);
    }
}
