#pragma once

#include <memory>

#include <vulkan/vulkan.h>

#include "../Handle.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderPass
    {
    public:
        explicit RenderPass(Instance* instance, const VkRenderPassCreateInfo& create_info);

        ~RenderPass();

        RenderPass(const RenderPass& other) = delete;

        RenderPass(RenderPass&& other) noexcept;

        RenderPass& operator=(const RenderPass& other) = delete;

        RenderPass& operator=(RenderPass&& other) noexcept;

        [[nodiscard]] VkRenderPass vk_handle() const;

    private:
        Instance* m_instance; // can be raw pointer, since the resource lives within the store on the instance
        VkDevice m_vk_device;
        VkRenderPass m_vk_handle{};


    };

    using RenderPassHandle = Handle<RenderPass>;
}
