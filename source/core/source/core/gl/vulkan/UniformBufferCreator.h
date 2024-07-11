#pragma once

#include <memory>
#include <cstdint>

#include "../enums.h"
#include "../Handle.h"
#include "../IUniformBufferCreator.h"

namespace yage::gl::vulkan
{
    class Instance;

    class UniformBufferCreator final : public IUniformBufferCreator
    {
    public:
        explicit UniformBufferCreator(std::weak_ptr<Instance> instance);

        [[nodiscard]] Handle<IUniformBuffer> create(std::size_t size, ResourceUsage usage) const override;

    private:
        std::weak_ptr<Instance> m_instance; // cannot be raw pointer, since the creator might outlive the instance
    };
}
