#pragma once

#include <span>
#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "../IDescriptorSetLayout.h"
#include "../IPipeline.h"
#include "../enums.h"

namespace yage::gl::vulkan
{
    class Instance;

    class Pipeline final : public IPipeline
    {
    public:
        Pipeline(Instance* instance, VkGraphicsPipelineCreateInfo& pipeline_info,
                 const VkPipelineLayoutCreateInfo& layout_info,
                 std::span<DescriptorSetLayoutHandle> descriptor_set_layouts,
                 std::span<VkDynamicState> dynamic_state);

        ~Pipeline() override;

        Pipeline(const Pipeline& other) = delete;

        Pipeline(Pipeline&& other) noexcept;

        Pipeline& operator=(const Pipeline& other) = delete;

        Pipeline& operator=(Pipeline&& other) noexcept;

        void set_dynamic_viewport(Viewport viewport, ScissorRectangle scissor) override;

        [[nodiscard]] VkPipeline vk_handle() const;

        [[nodiscard]] VkPipelineLayout vk_layout() const;

        void submit_dynamic_state(VkCommandBuffer command_buffer);

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;

        VkPipeline m_vk_handle{};
        VkPipelineLayout m_graphics_pipeline_layout{};

        std::vector<DescriptorSetLayoutHandle> m_descriptor_set_layouts;

        std::optional<VkViewport> m_dynamic_viewport;
        std::optional<VkRect2D> m_dynamic_scissor;

        void clear();
    };
}
