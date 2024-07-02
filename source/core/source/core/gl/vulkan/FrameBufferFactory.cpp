#include "FrameBufferFactory.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    FrameBufferFactory::FrameBufferFactory(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    std::unique_ptr<FrameBuffer> FrameBufferFactory::create_frame_buffer(std::shared_ptr<RenderPass> render_pass,
                                                                         std::span<ImageView*> attachements,
                                                                         const unsigned int width,
                                                                         const unsigned int height)
    {
        std::vector<VkImageView> vk_attachemtents;
        vk_attachemtents.reserve(attachements.size());
        std::ranges::transform(attachements, std::back_inserter(vk_attachemtents),
                               [](const ImageView* x) { return x->vk_handle(); });

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = render_pass->vk_handle();
        framebufferInfo.attachmentCount = vk_attachemtents.size();
        framebufferInfo.pAttachments = vk_attachemtents.data();
        framebufferInfo.width = width;
        framebufferInfo.height = height;
        framebufferInfo.layers = 1;

        return std::make_unique<FrameBuffer>(m_instance, std::move(render_pass), framebufferInfo);
    }
}
