#include "RenderPass.h"

namespace yage::gl::vulkan
{
    RenderPass::RenderPass(std::weak_ptr<Instance> instance)
        : m_instance(std::move(instance))
    {
    }

    VkRenderPass RenderPass::vk_get() const
    {
        return m_vk;
    }
}
