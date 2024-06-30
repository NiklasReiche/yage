#pragma once

#include <vulkan/vulkan.h>

#include "Instance.h"

namespace yage::gl::vulkan
{
    class RenderPass
    {
    public:
        explicit RenderPass(std::weak_ptr<Instance> instance);

        [[nodiscard]] VkRenderPass vk_get() const;

    private:
        std::weak_ptr<Instance> m_instance;
        VkRenderPass m_vk{};
    };
}
