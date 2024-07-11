#pragma once

#include "IDescriptorSetLayout.h"

namespace yage::gl
{
    class IDescriptorSetLayoutBuilder
    {
    public:
        virtual ~IDescriptorSetLayoutBuilder() = default;

        virtual IDescriptorSetLayoutBuilder& with_uniform_buffer_at(unsigned int binding) = 0;

        virtual DescriptorSetLayoutHandle build() = 0;

        virtual void clear() = 0;
    };
}
