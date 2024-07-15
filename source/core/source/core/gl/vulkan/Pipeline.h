#pragma once

#include <vulkan/vulkan.h>

#include "../Handle.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Pipeline final
    {
    public:
        Pipeline(Instance* instance, VkGraphicsPipelineCreateInfo& pipeline_info,
                 const VkPipelineLayoutCreateInfo& layout_info);

        ~Pipeline();

        Pipeline(const Pipeline& other) = delete;

        Pipeline(Pipeline&& other) noexcept;

        Pipeline& operator=(const Pipeline& other) = delete;

        Pipeline& operator=(Pipeline&& other) noexcept;

        [[nodiscard]] VkPipeline vk_handle() const;

        [[nodiscard]] VkPipelineLayout vk_layout() const;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;

        VkPipeline m_vk_handle{};
        VkPipelineLayout m_graphics_pipeline_layout{};
    };

    using PipelineHandle = Handle<Pipeline>;
}
