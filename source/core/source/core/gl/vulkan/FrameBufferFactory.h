#pragma once

#include <memory>
#include <span>

#include "../Handle.h"
#include "FrameBuffer.h"
#include "ImageView.h"

namespace yage::gl::vulkan
{
    class Instance;

    class FrameBufferFactory
    {
    public:
        explicit FrameBufferFactory(std::weak_ptr<Instance> instance);

        std::unique_ptr<FrameBuffer> create_frame_buffer(std::shared_ptr<Handle<RenderPass>> render_pass,
                                                         std::span<ImageView*> attachements,
                                                         unsigned int width,
                                                         unsigned int height);

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
