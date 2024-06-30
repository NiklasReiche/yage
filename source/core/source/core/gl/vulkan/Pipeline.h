#pragma once

#include <vulkan/vulkan.h>

#include "Instance.h"

namespace yage::gl::vulkan
{
    class Pipeline final
    {
    public:
        Pipeline(std::weak_ptr<Instance> instance, VkGraphicsPipelineCreateInfo& pipeline_info,
                 const VkPipelineLayoutCreateInfo& layout_info);

        ~Pipeline();

    private:
        std::weak_ptr<Instance> m_instance;
        VkPipeline m_graphics_pipeline{};
        VkPipelineLayout m_graphics_pipeline_layout{};
    };
}
