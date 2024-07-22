#include "Context.h"

namespace yage::gl::opengl4
{
    Context::Context(std::shared_ptr<platform::IWindow> window)
        : m_window(std::move(window))
    {
        populate_default_state();

        m_store_vertex_buffers = std::make_shared<VertexBufferStore>();
        m_store_index_buffers = std::make_shared<IndexBufferStore>();
        m_store_uniform_buffers = std::make_shared<UniformBufferStore>();
        m_store_drawables = std::make_shared<DrawableStore>();
        m_store_texture2ds = std::make_shared<Texture2DStore>();
        m_store_descriptor_sets = std::make_shared<DescriptorSetStore>();
        m_store_descriptor_set_layouts = std::make_shared<DescriptorSetLayoutStore>();
        m_store_pipelines = std::make_shared<PipelineStore>();
        m_store_render_targets = std::make_shared<RenderTargetStore>();

        m_vertex_buffer_creator = VertexBufferCreator(this);
        m_index_buffer_creator = IndexBufferCreator(this);
        m_uniform_buffer_creator = UniformBufferCreator(this);
        m_drawable_creator = DrawableCreator(this);
        m_texture2d_creator = Texture2DCreator(this);
        m_descriptor_set_creator = DescriptorSetCreator(this);
        m_descriptor_set_layout_builder = DescriptorSetLayoutBuilder(this);
        m_pipeline_builder = PipelineBuilder(this);
        m_render_target_builder = RenderTargetBuilder(this);
        m_renderer = Renderer(this);
    }

    IVertexBufferCreator& Context::vertex_buffer_creator()
    {
        return m_vertex_buffer_creator.value();
    }

    IIndexBufferCreator& Context::index_buffer_creator()
    {
        return m_index_buffer_creator.value();
    }

    IUniformBufferCreator& Context::uniform_buffer_creator()
    {
        return m_uniform_buffer_creator.value();
    }

    IDrawableCreator2& Context::drawable_creator()
    {
        return m_drawable_creator.value();
    }

    ITexture2DCreator& Context::texture_2d_creator()
    {
        return m_texture2d_creator.value();
    }

    IDescriptorSetCreator& Context::descriptor_set_creator()
    {
        return m_descriptor_set_creator.value();
    }

    IDescriptorSetLayoutBuilder& Context::descriptor_set_layout_builder()
    {
        return m_descriptor_set_layout_builder.value();
    }

    IPipelineBuilder& Context::pipeline_builder()
    {
        return m_pipeline_builder.value();
    }

    IRenderTargetBuilder& Context::render_target_builder()
    {
        return m_render_target_builder.value();
    }

    IRenderer2& Context::renderer()
    {
        return m_renderer.value();
    }

    VertexBufferStore& Context::store_vertex_buffers()
    {
        return *m_store_vertex_buffers;
    }

    IndexBufferStore& Context::store_index_buffers()
    {
        return *m_store_index_buffers;
    }

    UniformBufferStore& Context::store_uniform_buffers()
    {
        return *m_store_uniform_buffers;
    }

    DrawableStore& Context::store_drawables()
    {
        return *m_store_drawables;
    }

    Texture2DStore& Context::store_texture2ds()
    {
        return *m_store_texture2ds;
    }

    DescriptorSetStore& Context::store_descriptor_sets()
    {
        return *m_store_descriptor_sets;
    }

    DescriptorSetLayoutStore& Context::store_descriptor_set_layouts()
    {
        return *m_store_descriptor_set_layouts;
    }

    PipelineStore& Context::store_pipelines()
    {
        return *m_store_pipelines;
    }

    RenderTargetStore& Context::store_render_targets()
    {
        return *m_store_render_targets;
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

    void Context::bind_frame_buffer(const GLenum target, const GLuint frame_buffer)
    {
        if (target == GL_FRAMEBUFFER) {
            bind_frame_buffer(GL_READ_FRAMEBUFFER, frame_buffer);
            bind_frame_buffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
        } else {
            if (m_state.bound_framebuffers[target] != target) {
                glBindFramebuffer(target, frame_buffer);
                m_state.bound_framebuffers[target] = target;
            }
        }
    }

    void Context::force_bind_frame_buffer(const GLenum target, const GLuint frame_buffer)
    {
        if (target == GL_FRAMEBUFFER) {
            force_bind_frame_buffer(GL_READ_FRAMEBUFFER, frame_buffer);
            force_bind_frame_buffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
        } else {
            glBindFramebuffer(target, frame_buffer);
            m_state.bound_framebuffers[target] = target;
        }
    }

    void Context::unbind_frame_buffer(const GLenum target, const GLuint frame_buffer)
    {
        if (target == GL_FRAMEBUFFER) {
            unbind_frame_buffer(GL_READ_FRAMEBUFFER, frame_buffer);
            unbind_frame_buffer(GL_DRAW_FRAMEBUFFER, frame_buffer);
        } else {
            if (m_state.bound_framebuffers[target] == target) {
                glBindFramebuffer(target, frame_buffer);
                m_state.bound_framebuffers[target] = 0;
            }
        }
    }

    void Context::bind_render_buffer(const GLuint render_buffer)
    {
        if (m_state.bound_render_buffer != render_buffer) {
            glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
            m_state.bound_render_buffer = render_buffer;
        }
    }

    void Context::force_bind_render_buffer(const GLuint render_buffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
        m_state.bound_render_buffer = render_buffer;
    }

    void Context::unbind_render_buffer(const GLuint render_buffer)
    {
        if (m_state.bound_render_buffer == render_buffer) {
            glBindRenderbuffer(GL_RENDERBUFFER, render_buffer);
            m_state.bound_render_buffer = 0;
        }
    }

    void Context::bind_shader(const GLuint shader_program)
    {
        if (m_state.bound_shader != shader_program) {
            glUseProgram(shader_program);
            m_state.bound_shader = shader_program;
        }
    }

    void Context::force_bind_shader(const GLuint shader_program)
    {
        glUseProgram(shader_program);
        m_state.bound_shader = shader_program;
    }

    void Context::unbind_shader(const GLuint shader_program)
    {
        if (m_state.bound_shader == shader_program) {
            glUseProgram(0);
            m_state.bound_shader = 0;
        }
    }

    void Context::set_pixel_store_param(const GLenum param, const GLint value)
    {
        if (m_state.pixel_store_params[param] != value) {
            glPixelStorei(param, value);
            m_state.pixel_store_params[param] = value;
        }
    }

    void Context::set_viewport(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
    {
        const OpenGlRect viewport{x, y, width, height};
        if (m_state.viewport != viewport) {
            glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
            m_state.viewport = viewport;
        }
    }

    void Context::set_scissor(const GLint x, const GLint y, const GLsizei width, const GLsizei height)
    {
        const OpenGlRect scissor{x, y, width, height};
        if (m_state.scissor != scissor) {
            glScissor(scissor.x, scissor.y, scissor.width, scissor.height);
            m_state.scissor = scissor;
        }
    }

    void Context::set_capability_enabled(const GLenum capability, const bool enabled)
    {
        if (m_state.enabled_capabilities[capability] == enabled) {
            if (enabled) {
                glEnable(capability);
            } else {
                glDisable(capability);
            }
            m_state.enabled_capabilities[capability] = enabled;
        }
    }

    void Context::set_cull_mode(const GLenum mode)
    {
        if (m_state.cull_mode != mode) {
            glCullFace(mode);
            m_state.cull_mode = mode;
        }
    }

    void Context::set_polygon_mode(const GLenum mode)
    {
        if (m_state.polygon_mode != mode) {
            glPolygonMode(GL_FRONT_AND_BACK, mode);
            m_state.polygon_mode = mode;
        }
    }

    void Context::set_line_width(const GLfloat width)
    {
        if (m_state.line_width != width) {
            glLineWidth(width);
            m_state.line_width = width;
        }
    }

    void Context::set_blend_func(const GLenum sfactor, const GLenum dfactor)
    {
        if (m_state.blending_source_factor != sfactor || m_state.blending_destination_factor != dfactor) {
            glBlendFunc(sfactor, dfactor);
            m_state.blending_source_factor = sfactor;
            m_state.blending_destination_factor = dfactor;
        }
    }

    void Context::set_blend_equation(const GLenum op)
    {
        if (m_state.blending_operation != op) {
            glBlendEquation(op);
            m_state.blending_operation = op;
        }
    }

    void Context::set_blend_constant(const math::Vec4<GLfloat> color)
    {
        if (m_state.blending_constant != color) {
            glBlendColor(color(0), color(1), color(2), color(3));
            m_state.blending_constant = color;
        }
    }

    void Context::populate_default_state()
    {
        // texture and buffer bindings default to zero

        // capabilities default to disabled exepct for GL_MULTISAMPLE and GL_DITHER
        m_state.enabled_capabilities[GL_MULTISAMPLE] = true;
        m_state.enabled_capabilities[GL_DITHER] = true;

        // the other pixel store parameters all default to zero
        m_state.pixel_store_params[GL_PACK_ALIGNMENT] = 4;
        m_state.pixel_store_params[GL_UNPACK_ALIGNMENT] = 4;

        m_state.viewport.x = 0;
        m_state.viewport.y = 0;
        m_state.viewport.width = m_window->getWidth();
        m_state.viewport.height = m_window->getHeight();
        m_state.scissor.x = 0;
        m_state.scissor.y = 0;
        m_state.scissor.width = m_window->getWidth();
        m_state.scissor.height = m_window->getHeight();
    }
}
