#pragma once

#include <memory>
#include <vulkan/vulkan.h>

#include "../Handle.h"
#include "../TextureParams.h"
#include "RenderPass.h"

namespace yage::gl::vulkan
{
    class Instance;

    class RenderPassFactory
    {
    public:
        explicit RenderPassFactory(std::weak_ptr<Instance> instance);

        RenderPassHandle create(VkFormat image_format);

        RenderPassHandle create(ImageFormat image_format);

    private:
        std::weak_ptr<Instance> m_instance;
    };
}
