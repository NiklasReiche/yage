#include "DescriptorSetLayoutBuilder.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder(Context* context)
        : m_context(context)
    {
    }

    IDescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_uniform_buffer_at(unsigned)
    {
        return *this;
    }

    IDescriptorSetLayoutBuilder& DescriptorSetLayoutBuilder::with_texture_sampler_at(unsigned)
    {
        return *this;
    }

    DescriptorSetLayoutHandle DescriptorSetLayoutBuilder::build()
    {
        return m_context->store_descriptor_set_layouts().create();
    }

    void DescriptorSetLayoutBuilder::clear()
    {
    }
}
