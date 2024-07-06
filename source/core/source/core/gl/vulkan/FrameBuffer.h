#pragma once

#include <vulkan/vulkan.h>

#include <math/vector.h>

#include "../Handle.h"
#include "../IFrameBuffer.h"
#include "FrameCounter.h"
#include "ImageView.h"
#include "RenderPass.h"
#include "core/gl/enums.h"

namespace yage::gl::vulkan
{
    class Instance;

    class FrameBuffer final : public IFrameBuffer
    {
    public:
        explicit FrameBuffer(std::weak_ptr<Instance> instance, std::shared_ptr<RenderPassHandle> render_pass,
                             std::span<std::shared_ptr<ImageViewHandle>> attachements,
                             VkFramebufferCreateInfo create_info, FrameCounter frame_counter);

        ~FrameBuffer();

        FrameBuffer(const FrameBuffer& other) = delete;

        FrameBuffer(FrameBuffer&& other) noexcept;

        FrameBuffer& operator=(const FrameBuffer& other) = delete;

        FrameBuffer& operator=(FrameBuffer&& other) noexcept;

        [[nodiscard]] VkFramebuffer vk_handle() const;

        [[nodiscard]] VkFramebuffer vk_handle(std::size_t index) const;

        [[nodiscard]] unsigned int n_instances() const;

        [[nodiscard]] std::shared_ptr<RenderPassHandle> render_pass() const;

        [[nodiscard]] math::Vec2ui extent() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkDevice m_vk_device;
        std::shared_ptr<RenderPassHandle> m_render_pass;
        std::vector<std::shared_ptr<ImageViewHandle>> m_attachements;
        std::vector<VkFramebuffer> m_vk_handles{};
        math::Vec2ui m_extent{};
        FrameCounter m_frame_counter;
    };

    using FrameBufferHandle = Handle<IFrameBuffer>;
}
