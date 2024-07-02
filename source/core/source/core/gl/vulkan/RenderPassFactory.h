#pragma once

#include <memory>
#include <vulkan/vulkan.h>

#include "../TextureParams.h"
#include "../Handle.h"
#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderPassFactory
    {
    public:
        explicit RenderPassFactory(std::weak_ptr<Instance> instance);

        Handle<RenderPass> create(VkFormat image_format);

        Handle<RenderPass> create(ImageFormat image_format);

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
