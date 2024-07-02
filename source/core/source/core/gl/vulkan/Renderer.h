#pragma once

#include <vulkan/vulkan.h>

#include "Instance.h"
#include "FrameBuffer.h"
#include "Pipeline.h"

namespace yage::gl::vulkan
{
    class Renderer
    {
    public:
        explicit Renderer(std::weak_ptr<Instance> instance);

        void begin_command_buffer();

        void begin_render_pass(const FrameBuffer& frame_buffer);

        void bind_pipeline(const Pipeline& pipeline);

        void draw();

        void end_render_pass();

        void end_command_buffer();

    private:
        std::weak_ptr<Instance> m_instance;
        VkCommandBuffer m_command_buffer{};
    };
}
