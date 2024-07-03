#pragma once

#include <vulkan/vulkan.h>

#include <math/vector.h>

#include "../IFrameBuffer.h"
#include "../Handle.h"
#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class FrameBuffer final : public IFrameBuffer
    {
    public:
        explicit FrameBuffer(std::weak_ptr<Instance> instance, std::shared_ptr<RenderPassHandle> render_pass, VkFramebufferCreateInfo& create_info);

        ~FrameBuffer();

        FrameBuffer(const FrameBuffer& other) = delete;

        FrameBuffer(FrameBuffer&& other) noexcept;

        FrameBuffer& operator=(const FrameBuffer& other) = delete;

        FrameBuffer& operator=(FrameBuffer&& other) noexcept;

        [[nodiscard]] VkFramebuffer vk_handle() const;

        [[nodiscard]] std::shared_ptr<RenderPassHandle> render_pass() const;

        [[nodiscard]] math::Vec2ui extent() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkDevice m_vk_device;
        std::shared_ptr<RenderPassHandle> m_render_pass;
        VkFramebuffer m_vk_handle{};
        math::Vec2ui m_extent{};
    };

    using FrameBufferHandle = Handle<IFrameBuffer>;
}
