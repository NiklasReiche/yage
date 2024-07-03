#pragma once

#include <memory>
#include <span>

#include "../Handle.h"
#include "../IFrameBufferFactory.h"
#include "FrameBuffer.h"
#include "ImageView.h"

namespace yage::gl::vulkan
{
    class Instance;

    class FrameBufferFactory final : public IFrameBufferFactory
    {
    public:
        explicit FrameBufferFactory(std::weak_ptr<Instance> instance);

        FrameBufferHandle create_frame_buffer(std::shared_ptr<RenderPassHandle> render_pass,
                                                std::span<ImageViewHandle*> attachements, unsigned int width,
                                                unsigned int height);

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
