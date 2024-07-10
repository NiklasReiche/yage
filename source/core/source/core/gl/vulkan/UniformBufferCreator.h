#pragma once

#include <memory>
#include <cstdint>

#include "../enums.h"
#include "../Handle.h"
#include "UniformBuffer.h"

namespace yage::gl::vulkan
{
    class Instance;

    class UniformBufferCreator final
    {
    public:
        explicit UniformBufferCreator(std::weak_ptr<Instance> instance);

        [[nodiscard]] Handle<UniformBuffer> create(std::uint32_t size, ResourceUsage usage) const;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
    };
}
