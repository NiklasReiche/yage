#pragma once

#include <vulkan/vulkan.h>

#include "../IDrawable.h"
#include "DescriptorSet.h"
#include "Pipeline.h"
#include "RenderTarget.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Renderer
    {
    public:
        explicit Renderer(std::weak_ptr<Instance> instance);

        void begin_command_buffer();

        void begin_render_pass();

        void begin_render_pass(const RenderTarget& render_target);

        void bind_pipeline(const IPipeline& pipeline, const IDescriptorSet& descriptor_set);

        void draw(const IDrawable2& drawable);

        void end_render_pass();

        void end_command_buffer();

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
        VkCommandBuffer m_command_buffer{};

        bool m_render_to_swap_chain = false;
    };
}
