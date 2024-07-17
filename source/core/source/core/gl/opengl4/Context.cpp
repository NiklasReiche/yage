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

    void Context::unbind_buffer(const GLenum target, const GLuint buffer)
    {
        if (m_state.bound_buffer_per_target[target] == buffer) {
            bind_buffer(target, 0);
        }
    }

    void Context::bind_vao(const GLuint vao)
    {
        if (m_state.bound_vao != vao) {
            glBindVertexArray(vao);
            m_state.bound_vao = vao;
        }
    }

    void Context::unbind_vao(const GLuint vao)
    {
        if (m_state.bound_vao == vao) {
            bind_vao(0);
        }
    }
}
