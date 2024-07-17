#include "Context.h"

namespace yage::gl::opengl4
{
    void Context::bind_buffer(const GLenum target, const GLuint buffer)
    {
        if (m_state.bound_buffer_per_target[target] != buffer) {
            glBindBuffer(target, buffer);
            m_state.bound_buffer_per_target[target] = buffer;
        }
    }
}
