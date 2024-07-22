#include "DescriptorSetCreator.h"

namespace yage::gl::opengl4
{
    DescriptorSetCreator::DescriptorSetCreator(Context* context)
        : m_context(context)
    {
    }

    DescriptorSetHandle DescriptorSetCreator::create(DescriptorSetLayoutHandle, ResourceUsage)
    {
        // TODO
        return DescriptorSetHandle();
    }
}
