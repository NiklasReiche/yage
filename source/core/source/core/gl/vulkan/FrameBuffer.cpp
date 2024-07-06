#include "FrameBuffer.h"
#include "Instance.h"

namespace yage::gl::vulkan
{
    FrameBuffer::FrameBuffer(std::weak_ptr<Instance> instance, std::shared_ptr<RenderPassHandle> render_pass,
                             std::span<std::shared_ptr<ImageViewHandle>> attachements,
                             VkFramebufferCreateInfo create_info, const FrameCounter frame_counter)
        : m_instance(std::move(instance)),
          m_vk_device(m_instance.lock()->device()),
          m_render_pass(std::move(render_pass)),
          m_extent({create_info.width, create_info.height}),
          m_frame_counter(frame_counter)
    {
        const std::size_t n_instances = m_frame_counter.max_frame_index;

        m_attachements.resize(attachements.size());
        std::ranges::move(attachements, m_attachements.begin());

        for (const std::shared_ptr<ImageViewHandle>& attachement: m_attachements) {
            assert(attachement->get<ImageView>().n_instances() == n_instances);
        }

        m_vk_handles.resize(n_instances);
        std::vector<VkImageView> vk_attachements;
        vk_attachements.resize(m_attachements.size());
        create_info.attachmentCount = m_attachements.size();
        create_info.renderPass = m_render_pass->get<RenderPass>().vk_handle();
        for (std::size_t i = 0; i < n_instances; ++i) {
            for (std::size_t j = 0; j < m_attachements.size(); j++) {
                vk_attachements[j] = m_attachements[j]->get<ImageView>().vk_handle(i);
            }
            create_info.pAttachments = vk_attachements.data();

            if (vkCreateFramebuffer(m_vk_device, &create_info, nullptr, &m_vk_handles[i]) != VK_SUCCESS) {
                throw std::runtime_error("Vulkan: failed to create frame buffer!");
            }
        }
    }

    FrameBuffer::~FrameBuffer()
    {
        for (const VkFramebuffer handle: m_vk_handles) {
            if (handle != VK_NULL_HANDLE) {
                vkDestroyFramebuffer(m_vk_device, handle, nullptr);
            }
        }
    }

    FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
        : m_instance(std::move(other.m_instance)),
          m_vk_device(other.m_vk_device),
          m_render_pass(std::move(other.m_render_pass)),
          m_vk_handles(std::move(other.m_vk_handles)),
          m_extent(other.m_extent),
          m_frame_counter(other.m_frame_counter)
    {
        other.m_vk_device = VK_NULL_HANDLE;
        for (VkFramebuffer& m_vk_handle: other.m_vk_handles) {
            m_vk_handle = VK_NULL_HANDLE;
        }
        other.m_extent = {0, 0};
    }

    FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_instance = std::move(other.m_instance);
        m_vk_device = other.m_vk_device;
        m_render_pass = std::move(other.m_render_pass);
        m_vk_handles = other.m_vk_handles;
        m_extent = other.m_extent;
        m_frame_counter = other.m_frame_counter;

        other.m_vk_device = VK_NULL_HANDLE;
        for (VkFramebuffer& m_vk_handle: other.m_vk_handles) {
            m_vk_handle = VK_NULL_HANDLE;
        }
        other.m_extent = {0, 0};

        return *this;
    }

    VkFramebuffer FrameBuffer::vk_handle() const
    {
        return m_vk_handles[*m_frame_counter.curent_frame_index];
    }

    VkFramebuffer FrameBuffer::vk_handle(const std::size_t index) const
    {
        return m_vk_handles[index];
    }

    unsigned int FrameBuffer::n_instances() const
    {
        return m_frame_counter.max_frame_index;
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
