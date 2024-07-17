#pragma once

#include <unordered_map>

#include <glad/gl.h>

namespace yage::gl::opengl4
{
    class Context
    {
    public:
        void bind_buffer(GLenum target, GLuint buffer);

    private:
        struct OpenGlState
        {
            std::unordered_map<GLenum, GLuint> bound_buffer_per_target;
        };

        OpenGlState m_state;
    };
}
