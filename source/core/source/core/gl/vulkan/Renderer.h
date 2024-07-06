#pragma once

#include <vulkan/vulkan.h>

#include "FrameBuffer.h"
#include "Pipeline.h"
#include "../IDrawable.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Renderer
    {
    public:
        explicit Renderer(std::weak_ptr<Instance> instance);

        void begin_command_buffer();

        void begin_render_pass(const FrameBuffer& frame_buffer);

        void bind_pipeline(const Pipeline& pipeline);

        void draw(const IDrawable2& drawable);

        void end_render_pass();

        void end_command_buffer();

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
        VkCommandBuffer m_command_buffer{};
    };
}
