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
        Pipeline(std::weak_ptr<Instance> instance, std::shared_ptr<RenderPassHandle> render_pass,
                 VkGraphicsPipelineCreateInfo& pipeline_info,
                 const VkPipelineLayoutCreateInfo& layout_info);

        ~Pipeline();

        Pipeline(const Pipeline& other) = delete;

        Pipeline(Pipeline&& other) noexcept;

        Pipeline& operator=(const Pipeline& other) = delete;

        Pipeline& operator=(Pipeline&& other) noexcept;

        [[nodiscard]] VkPipeline vk_handle() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkDevice m_vk_device;
        std::shared_ptr<RenderPassHandle> m_render_pass;
        VkPipeline m_vk_handle{};
        VkPipelineLayout m_graphics_pipeline_layout{};
    };

    using PipelineHandle = Handle<Pipeline>;
}
