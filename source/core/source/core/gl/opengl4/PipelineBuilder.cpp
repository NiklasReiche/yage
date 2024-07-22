#include "PipelineBuilder.h"

#include "Context.h"
#include "enums.h"

namespace yage::gl::opengl4
{
    PipelineBuilder::PipelineBuilder(Context* context)
        : m_context(context)
    {
    }

    IPipelineBuilder& PipelineBuilder::with_shaders(const ShaderSource vertex_code, const ShaderSource fragment_code)
    {
        const char* vertex_source = std::get<const std::string_view>(vertex_code).data();
        const GLuint vertex_module = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_module, 1, &vertex_source, nullptr);
        glCompileShader(vertex_module);
        check_shader_model_compilation_error(vertex_module);
        m_descriptor.shader_modules.push_back(vertex_module);

        const char* fragment_source = std::get<const std::string_view>(fragment_code).data();
        const GLuint fragment_module = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_module, 1, &fragment_source, nullptr);
        glCompileShader(fragment_module);
        check_shader_model_compilation_error(fragment_module);
        m_descriptor.shader_modules.push_back(fragment_module);

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_shaders(const ShaderSource vertex_code, const ShaderSource fragment_code,
                                                    const ShaderSource geometry_code)
    {
        with_shaders(vertex_code, fragment_code);

        const char* geometry_source = std::get<const std::string_view>(geometry_code).data();
        const GLuint geometry_module = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry_module, 1, &geometry_source, nullptr);
        glCompileShader(geometry_module);
        check_shader_model_compilation_error(geometry_module);
        m_descriptor.shader_modules.push_back(geometry_module);

        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_vertex_format(const PrimitiveTopology topology, VertexDataInfo)
    {
        m_descriptor.primitive = topology;
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::append_layout(DescriptorSetLayoutHandle)
    {
        // TODO
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_rasterizer(const PolygonMode polygon_mode, const CullMode cull_mode,
                                                       const float line_width)
    {
        m_descriptor.polygon_mode = polygon_mode;
        m_descriptor.cull_mode = cull_mode;
        m_descriptor.line_width = line_width;
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_viewport(Viewport viewport, ScissorRectangle scissor)
    {
        m_descriptor.viewport = viewport;
        m_descriptor.scissor = scissor;
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_dynamic_viewport()
    {
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::for_render_target(const IRenderTarget&)
    {
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::for_swap_chain_render_target()
    {
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_depth_test()
    {
        m_descriptor.with_depth_test = true;
        return *this;
    }

    IPipelineBuilder& PipelineBuilder::with_blending(BlendInfo blend_info)
    {
        m_descriptor.blending = blend_info;
        return *this;
    }

    PipelineHandle PipelineBuilder::build()
    {
        return m_context->store_pipelines().create(m_context, m_descriptor);
    }

    void PipelineBuilder::clear()
    {
        destroy_shader_modules();

        m_descriptor = PipelineDescriptor{};
    }

    void PipelineBuilder::destroy_shader_modules()
    {
        for (const GLuint shader_module: m_descriptor.shader_modules) {
            glDeleteShader(shader_module);
        }
        m_descriptor.shader_modules.clear();
    }

    void PipelineBuilder::check_shader_model_compilation_error(const GLuint module)
    {
        int success;
        glGetShaderiv(module, GL_COMPILE_STATUS, &success);
        if (!success) {
            // TODO: dynamically sized log
            constexpr int error_log_byte_size = 1024;
            char infoLog[error_log_byte_size];
            glGetShaderInfoLog(module, error_log_byte_size, nullptr, infoLog);
            throw std::runtime_error(infoLog);
        }
    }
}
