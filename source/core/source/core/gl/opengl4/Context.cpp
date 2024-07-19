#include "Context.h"

namespace yage::gl::opengl4
{
    Context::Context()
    {
        populate_default_state();
    }

    void Context::bind_buffer(const GLenum target, const GLuint buffer)
    {
        if (m_state.bound_buffer_per_target[target] != buffer) {
            glBindBuffer(target, buffer);
            m_state.bound_buffer_per_target[target] = buffer;
        }
    }

    void Context::force_bind_buffer(const GLenum target, const GLuint buffer)
    {
        glBindBuffer(target, buffer);
        m_state.bound_buffer_per_target[target] = buffer;
    }

    void Context::unbind_buffer(const GLenum target, const GLuint buffer)
    {
        if (m_state.bound_buffer_per_target[target] == buffer) {
            force_bind_buffer(target, 0);
        }
    }

    void Context::bind_texture(const GLenum target, const GLuint texture)
    {
        if (m_state.bound_texture_per_target[target] != texture) {
            glBindTexture(target, texture);
            m_state.bound_texture_per_target[target] = texture;
        }
    }

    void Context::force_bind_texture(const GLenum target, const GLuint texture)
    {
        glBindTexture(target, texture);
        m_state.bound_texture_per_target[target] = texture;
    }

    void Context::unbind_texture(const GLenum target, const GLuint texture)
    {
        if (m_state.bound_texture_per_target[target] == texture) {
            force_bind_texture(target, 0);
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

    void Context::set_pixel_store_param(const GLenum param, const GLint value)
    {
        if (m_state.pixel_store_params[param] != value) {
            glPixelStorei(param, value);
            m_state.pixel_store_params[param] = value;
        }
    }

    void Context::populate_default_state()
    {
        // texture and buffer bindings default to zero

        m_state.bound_vao = 0;

        // the other pixel store parameters all default to zero
        m_state.pixel_store_params[GL_PACK_ALIGNMENT] = 4;
        m_state.pixel_store_params[GL_UNPACK_ALIGNMENT] = 4;
    }
}
