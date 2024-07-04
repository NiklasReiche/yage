#pragma once

#include <memory>
#include <span>

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

        FrameBufferHandle create(std::shared_ptr<RenderPassHandle> render_pass,
                                                std::span<ImageViewHandle*> attachements, unsigned int width,
                                                unsigned int height);

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
