#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Pipeline final
    {
    public:
        Pipeline(std::weak_ptr<Instance> instance, std::shared_ptr<Handle<RenderPass>> render_pass,
                 VkGraphicsPipelineCreateInfo& pipeline_info,
                 const VkPipelineLayoutCreateInfo& layout_info);

        ~Pipeline();

        [[nodiscard]] VkPipeline vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance;
        std::shared_ptr<Handle<RenderPass>> m_render_pass;
        VkPipeline m_vk_handle{};
        VkPipelineLayout m_graphics_pipeline_layout{};
    };
}
