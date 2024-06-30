#include "FrameBuffer.h"

namespace yage::gl::vulkan {
    FrameBuffer::FrameBuffer(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    std::shared_ptr<RenderPass> FrameBuffer::render_pass() const
    {
        return m_render_pass;
    }
}
