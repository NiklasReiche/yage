#pragma once

#include <vulkan/vulkan.h>

#include "../IDrawable.h"
#include "../IRenderer.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Renderer final : public IRenderer2
    {
    public:
        explicit Renderer(std::weak_ptr<Instance> instance);

        void begin_command_buffer();

        void begin_render_pass() override;

        void begin_render_pass(const IRenderTarget& render_target) override;

        void bind_descriptor_set(const IDescriptorSet& descriptor_set) override;

        void bind_pipeline(const IPipeline& pipeline) override;

        void draw(const IDrawable2& drawable) override;

        void end_render_pass() override;

        void end_command_buffer();

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
        VkCommandBuffer m_command_buffer{};

        std::vector<VkDescriptorSet> m_descriptor_sets_to_bind;

        bool m_render_to_swap_chain = false;
    };
}
