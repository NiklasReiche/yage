#pragma once

#include "../IDescriptorSetLayoutBuilder.h"

namespace yage::gl::opengl4
{
    class Context;

    class DescriptorSetLayoutBuilder final : public IDescriptorSetLayoutBuilder
    {
    public:
        explicit DescriptorSetLayoutBuilder(Context* context);

        IDescriptorSetLayoutBuilder& with_uniform_buffer_at(unsigned binding) override;

        IDescriptorSetLayoutBuilder& with_texture_sampler_at(unsigned binding) override;

        DescriptorSetLayoutHandle build() override;

        void clear() override;

    private:
        Context* m_context; // TODO
    };
}
