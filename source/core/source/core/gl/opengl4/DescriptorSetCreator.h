#pragma once

#include "../IDescriptorSetCreator.h"

namespace yage::gl::opengl4
{
    class Context;

    class DescriptorSetCreator final : public IDescriptorSetCreator
    {
    public:
        explicit DescriptorSetCreator(Context* context);

        DescriptorSetHandle create(DescriptorSetLayoutHandle layout, ResourceUsage usage) override;

    private:
        Context* m_context; // TODO
    };
}
