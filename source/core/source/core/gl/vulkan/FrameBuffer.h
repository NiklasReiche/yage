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
        explicit FrameBuffer(Instance* instance, RenderPassHandle render_pass,
                             std::span<ImageViewHandle> attachements,
                             VkFramebufferCreateInfo create_info, FrameCounter frame_counter);

        ~FrameBuffer() override;

        FrameBuffer(const FrameBuffer& other) = delete;

        FrameBuffer(FrameBuffer&& other) noexcept;

        FrameBuffer& operator=(const FrameBuffer& other) = delete;

        FrameBuffer& operator=(FrameBuffer&& other) noexcept;

        [[nodiscard]] VkFramebuffer vk_handle() const;

        [[nodiscard]] VkFramebuffer vk_handle(std::size_t index) const;

        [[nodiscard]] unsigned int n_instances() const;

        [[nodiscard]] const RenderPassHandle& render_pass() const;

        [[nodiscard]] math::Vec2ui extent() const;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        FrameCounter m_frame_counter;

        RenderPassHandle m_render_pass;
        std::vector<ImageViewHandle> m_attachements;
        std::vector<VkFramebuffer> m_vk_handles{};
        math::Vec2ui m_extent{};
    };
}
