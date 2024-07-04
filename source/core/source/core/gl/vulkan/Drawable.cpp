#include "Drawable.h"

namespace yage::gl::vulkan
{
    Drawable::Drawable(const DrawableDescriptor& descriptor)
        : m_descriptor(descriptor)
    {
    }

    const DrawableDescriptor& Drawable::descriptor() const
    {
        return m_descriptor;
    }

    VertexDataInfo Drawable::vertex_data_info() const
    {
        return m_descriptor.vertex_data_info;
    }
}
