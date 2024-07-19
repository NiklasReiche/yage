#pragma once

#include <unordered_map>

#include <glad/gl.h>

namespace yage::gl::opengl4
{
    class Context
    {
    public:
        Context();

        void bind_buffer(GLenum target, GLuint buffer);

        void force_bind_buffer(GLenum target, GLuint buffer);

        void unbind_buffer(GLenum target, GLuint buffer);

        void bind_texture(GLenum target, GLuint texture);

        void force_bind_texture(GLenum target, GLuint texture);

        void unbind_texture(GLenum target, GLuint texture);

        void bind_vao(GLuint vao);

        void unbind_vao(GLuint vao);

        void set_pixel_store_param(GLenum param, GLint value);

    private:
        struct OpenGlState
        {
            std::unordered_map<GLenum, GLuint> bound_buffer_per_target;
            std::unordered_map<GLenum, GLuint> bound_texture_per_target;
            GLuint bound_vao;

            std::unordered_map<GLenum, GLint> pixel_store_params;
        };

        OpenGlState m_state{};

        void populate_default_state();
    };
}
