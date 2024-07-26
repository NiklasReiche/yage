#include <memory>

#include <core/platform/desktop/GlfwWindow.h>

#include <core/gl/vulkan/Instance.h>
#include <core/gl/opengl4/Context.h>

#include "math/math.h"

using namespace yage;

struct UniformBufferData
{
    math::Mat4f projection;
    math::Mat4f view;
    math::Mat4f model;
};

int main()
{
    const std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(
            500, 500, "Hello Triangle", platform::desktop::GlfwWindow::GlApi::API_OPENGL);
    const std::shared_ptr<gl::IContext> context = window->gl_context().lock();

    const gl::IDrawableCreator2& drawable_creator = context->drawable_creator();

    const gl::VertexDataInfo vertex_data_info{
            .vertex_description =
                    {
                            {gl::VertexComponentSize::VEC3, gl::VertexComponentType::FLOAT_32},
                            {gl::VertexComponentSize::VEC3, gl::VertexComponentType::FLOAT_32},
                    },
            .data_layout = gl::VertexDataLayout::INTERLEAVED,
    };
    /* clang-format off */
    constexpr std::array<const float, 48> vertex_data = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };
    /* clang-format on */
    constexpr gl::IndexDataInfo index_data_info{
            .data_type = gl::IndexType::UINT_16,
            .index_count = 12,
    };
    constexpr std::array<std::uint16_t, 12> index_data = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4};
    const gl::DrawableHandle drawable =
            drawable_creator.create(vertex_data_info, std::as_bytes(std::span{vertex_data}), index_data_info,
                                    std::as_bytes(std::span{index_data}), gl::ResourceUsage::STATIC);

    const gl::IUniformBufferCreator& uniform_buffer_creator = context->uniform_buffer_creator();
    const gl::UniformBufferHandle ubo =
            uniform_buffer_creator.create(sizeof(UniformBufferData), gl::ResourceUsage::DYNAMIC);

    gl::IDescriptorSetLayoutBuilder& descriptor_set_layout_builder = context->descriptor_set_layout_builder();
    const gl::DescriptorSetLayoutHandle layout = descriptor_set_layout_builder.with_uniform_buffer_at(0).build();

    gl::IDescriptorSetCreator& descriptor_set_creator = context->descriptor_set_creator();
    const gl::DescriptorSetHandle descriptor_set = descriptor_set_creator.create(layout, gl::ResourceUsage::DYNAMIC);
    descriptor_set->write(0, ubo);

    const auto file_reader = window->getFileReader();
    auto vert_code = file_reader->openTextFile(R"(assets/shader.vert)", platform::IFile::AccessMode::READ)->readAll();
    auto frag_code = file_reader->openTextFile(R"(assets/shader.frag)", platform::IFile::AccessMode::READ)->readAll();
    const gl::PipelineHandle graphics_pipeline =
            context->pipeline_builder()
                    .with_shaders(vert_code, frag_code)
                    .with_vertex_format(gl::PrimitiveTopology::TRIANGLE_LIST, vertex_data_info)
                    .append_layout(layout)
                    .with_rasterizer(gl::PolygonMode::FILL, gl::CullMode::NONE, 1.0f)
                    .with_dynamic_viewport()
                    .for_swap_chain_render_target()
                    .with_depth_test()
                    .build();

    graphics_pipeline->set_dynamic_viewport({0, 0, 500.0f, 500.0f}, {0, 0, 500, 500});
    window->attach_on_framebuffer_resize([&graphics_pipeline](const int width, const int height) {
        graphics_pipeline->set_dynamic_viewport(
                {0, 0, static_cast<float>(width), static_cast<float>(height)},
                {0, 0, static_cast<unsigned int>(width), static_cast<unsigned int>(width)});
    });

    gl::IRenderer2& renderer = context->renderer();

    UniformBufferData ubo_data;
    ubo_data.projection = transpose(math::matrix::perspective(90.0f, 1.0f, 0.1f, 100.f));
    ubo_data.view = transpose(math::matrix::look_at(math::Vec3f(0, 1, 0.5), math::Vec3f(0, 0, 0)));
    ubo_data.model = transpose(math::matrix::translate(0.f, 0.f, 0.f));

    //ubo_data.projection(1, 1) *= -1;

    window->show();
    while (!window->shouldDestroy()) {
        window->pollEvents();

        ubo->update_sub_data(0, sizeof(UniformBufferData), &ubo_data);

        context->prepare_frame();

        renderer.begin_command_buffer();
        renderer.begin_render_pass();
        renderer.bind_descriptor_set(*descriptor_set);
        renderer.bind_pipeline(*graphics_pipeline);
        renderer.draw(*drawable);
        renderer.end_render_pass();
        renderer.end_command_buffer();

        context->present_frame();
    }

    //context->flush_gpu();
    return 0;
}
