#pragma once

#include <vulkan/vulkan.h>

#include <math/vector.h>

#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class FrameBuffer final
    {
    public:
        explicit FrameBuffer(std::weak_ptr<Instance> instance, std::shared_ptr<RenderPass> render_pass, VkFramebufferCreateInfo& create_info);

        ~FrameBuffer();

        [[nodiscard]] VkFramebuffer vk_handle() const;

        [[nodiscard]] std::shared_ptr<RenderPass> render_pass() const;

        [[nodiscard]] math::Vec2ui extent() const;

    private:
        std::weak_ptr<Instance> m_instance;
        std::shared_ptr<RenderPass> m_render_pass;
        VkFramebuffer m_vk_handle{};
        math::Vec2ui m_extent{};
    };
}
