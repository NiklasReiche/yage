#include "DescriptorSet.h"
#include <glad/gl.h>

#include "Texture2D.h"
#include "UniformBuffer.h"

namespace yage::gl::opengl4
{
    void DescriptorSet::write(const unsigned int binding, const UniformBufferHandle& uniform_buffer)
    {
        // we use layout(binding) syntax in shaders, so no binding is necessary here
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, uniform_buffer.get<UniformBuffer>().gl_handle());
    }

    void DescriptorSet::write(const unsigned int binding, const Texture2DHandle& texture_2d)
    {
        // we use layout(binding) syntax in shaders, so no unifrom setting is necessary here
        m_samplers.emplace(binding, texture_2d);
    }

    void DescriptorSet::bind_state() const
    {
        for (const auto&[binding, texture]: m_samplers) {
            glActiveTexture(GL_TEXTURE0 + binding);
            glBindTexture(GL_TEXTURE_2D, texture.get<Texture2D>().gl_handle());
        }
    }
}
