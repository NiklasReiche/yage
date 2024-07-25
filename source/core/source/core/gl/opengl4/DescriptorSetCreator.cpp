#include "DescriptorSetCreator.h"

#include "Context.h"

namespace yage::gl::opengl4
{
    DescriptorSetCreator::DescriptorSetCreator(Context* context)
        : m_context(context)
    {
    }

    DescriptorSetHandle DescriptorSetCreator::create(DescriptorSetLayoutHandle, ResourceUsage)
    {
        return m_context->store_descriptor_sets().create();
    }
}
