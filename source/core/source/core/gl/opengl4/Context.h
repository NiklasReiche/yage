#pragma once

#include <unordered_map>

#include <glad/gl.h>

namespace yage::gl::opengl4
{
    class Context
    {
    public:
        void bind_buffer(GLenum target, GLuint buffer);

        void force_bind_buffer(GLenum target, GLuint buffer);

        void unbind_buffer(GLenum target, GLuint buffer);

        void bind_vao(GLuint vao);

        void unbind_vao(GLuint vao);

    private:
        struct OpenGlState
        {
            std::unordered_map<GLenum, GLuint> bound_buffer_per_target;
            GLuint bound_vao = 0;
        };

        OpenGlState m_state{};
    };
}
