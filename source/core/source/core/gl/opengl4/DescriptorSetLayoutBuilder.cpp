#include "DescriptorSetLayoutBuilder.h"

namespace yage::gl::opengl4
{
    DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder(Context* context)
        : m_context(context)
    {
    }

    IDescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_uniform_buffer_at(unsigned)
    {
        // TODO
        return *this;
    }

    IDescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_texture_sampler_at(unsigned)
    {
        // TODO
        return *this;
    }

    DescriptorSetLayoutHandle DescriptorSetLayoutBuilder::build()
    {
        // TODO
        return DescriptorSetLayoutHandle();
    }

    void DescriptorSetLayoutBuilder::clear()
    {
        // TODO
    }
}
