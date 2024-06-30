#pragma once

#include <vulkan/vulkan.h>

#include "Instance.h"
#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class FrameBuffer final
    {
    public:
        explicit FrameBuffer(std::weak_ptr<Instance> instance);

        [[nodiscard]] std::shared_ptr<RenderPass> render_pass() const;

    private:
        std::weak_ptr<Instance> m_instance;
        std::shared_ptr<RenderPass> m_render_pass;
    };
}
