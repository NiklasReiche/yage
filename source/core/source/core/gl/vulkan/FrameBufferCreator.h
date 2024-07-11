#pragma once

#include <memory>
#include <span>
#include <cstdint>

#include "../Handle.h"
#include "../IFrameBufferCreator.h"
#include "FrameBuffer.h"
#include "ImageView.h"

namespace yage::gl::vulkan
{
    class Instance;

    class FrameBufferCreator final : public IFrameBufferCreator
    {
    public:
        explicit FrameBufferCreator(std::weak_ptr<Instance> instance);

        FrameBufferHandle create(RenderPassHandle render_pass,
                                 std::span<ImageViewHandle> attachements, std::uint32_t width,
                                 std::uint32_t height, ResourceUsage usage);

        FrameBufferHandle create_swap_chain(RenderPassHandle render_pass,
                                 std::span<ImageViewHandle> attachements, std::uint32_t width,
                                 std::uint32_t height);

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
    };
}
