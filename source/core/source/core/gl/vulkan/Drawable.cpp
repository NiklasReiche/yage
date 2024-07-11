#include "Drawable.h"

#include <utility>

namespace yage::gl::vulkan
{
    Drawable::Drawable(DrawableDescriptor descriptor)
        : m_descriptor(std::move(descriptor))
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
