#include "Pipeline.h"
#include "Context.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    Pipeline::Pipeline(Context* context, const PipelineDescriptor& descriptor)
        : m_context(context)
    {
        m_shader_program = glCreateProgram();
        for (const GLuint module: descriptor.shader_modules) {
            glAttachShader(m_shader_program, module);
        }
        glLinkProgram(m_shader_program);
        check_shader_link_error(m_shader_program);

        m_primitive = convert(descriptor.primitive);
        m_polygon_mode = convert(descriptor.polygon_mode);
        if (descriptor.cull_mode != CullMode::NONE) {
            m_cull_mode = convert(descriptor.cull_mode);
        }
        m_line_width = descriptor.line_width;
        m_viewport = descriptor.viewport;
        m_scissor = descriptor.scissor;
        m_depth_test = descriptor.with_depth_test;
    }

    Pipeline::~Pipeline()
    {
        clear();
    }

    Pipeline::Pipeline(Pipeline&& other) noexcept
        : m_context(other.m_context),
          m_shader_program(other.m_shader_program),
          m_primitive(other.m_primitive),
          m_polygon_mode(other.m_polygon_mode),
          m_cull_mode(other.m_cull_mode),
          m_line_width(other.m_line_width),
          m_viewport(other.m_viewport),
          m_scissor(other.m_scissor),
          m_depth_test(other.m_depth_test),
          m_blend_state(other.m_blend_state)
    {
        other.m_context = nullptr;
        other.m_shader_program = 0;
    }

    Pipeline& Pipeline::operator=(Pipeline&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_context = other.m_context;
        m_shader_program = other.m_shader_program;
        m_primitive = other.m_primitive;
        m_polygon_mode = other.m_polygon_mode;
        m_cull_mode = other.m_cull_mode;
        m_line_width = other.m_line_width;
        m_viewport = other.m_viewport;
        m_scissor = other.m_scissor;
        m_depth_test = other.m_depth_test;
        m_blend_state = other.m_blend_state;

        other.m_context = nullptr;
        other.m_shader_program = 0;

        return *this;
    }

    void Pipeline::set_dynamic_viewport(const Viewport viewport, const ScissorRectangle scissor)
    {
        m_viewport = viewport;
        m_scissor = scissor;
    }

    void Pipeline::bind_state()
    {
        // TODO: this would also submit for a static pipeline on which set_dynamic_viewport was called
        if (m_viewport.has_value()) {
            m_context->set_viewport(m_viewport->x, m_viewport->y, m_viewport->width, m_viewport->height);
        }
        if (m_scissor.has_value()) {
            m_context->set_scissor(m_scissor->x, m_scissor->y, m_scissor->width, m_scissor->height);
        }

        m_context->set_polygon_mode(m_polygon_mode);

        if (m_cull_mode) {
            m_context->set_capability_enabled(GL_CULL_FACE, true);
            m_context->set_cull_mode(m_cull_mode.value());
        } else {
            m_context->set_capability_enabled(GL_CULL_FACE, false);
        }

        m_context->set_line_width(m_line_width);

        m_context->set_capability_enabled(GL_DEPTH_TEST, m_depth_test);

        if (m_blend_state.has_value()) {
            m_context->set_capability_enabled(GL_BLEND, true);
            m_context->set_blend_func(m_blend_state->source_factor, m_blend_state->destination_factor);
            m_context->set_blend_equation(m_blend_state->operation);
            m_context->set_blend_constant(m_blend_state->constant);
        } else {
            m_context->set_capability_enabled(GL_BLEND, false);
        }
    }

    GLenum Pipeline::primitive() const
    {
        return m_primitive;
    }

    void Pipeline::clear()
    {
        if (m_context != nullptr) {
            glDeleteProgram(m_shader_program);
            m_context->unbind_shader(m_shader_program);
        }
    }

    void Pipeline::check_shader_link_error(const GLuint shader_program)
    {
        int success;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            // TODO: dynamically sized log
            constexpr int error_log_byte_size = 1024;
            char infoLog[error_log_byte_size];
            glGetProgramInfoLog(shader_program, error_log_byte_size, nullptr, infoLog);
            throw std::runtime_error(infoLog);
        }
    }
}
