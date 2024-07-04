#include <memory>

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>

#include <core/gl/vulkan/Instance.h>
#include <core/gl/vulkan/PipelineBuilder.h>
#include <core/gl/vulkan/Renderer.h>

#include "core/gl/vulkan/VertexBufferCreator.h"

using namespace yage;

gl::vulkan::PipelineHandle create_pipeline(platform::IWindow& window, const std::weak_ptr<gl::vulkan::Instance>& context)
{
    const auto file_reader = window.getFileReader();
    auto vert_code = file_reader->openBinaryFile(
        R"(C:\Users\Niklas\Code\yage\demo\SimpleWindow\assets\vert.spv)",
        platform::IFile::AccessMode::READ)->read_all();
    auto frag_code = file_reader->openBinaryFile(
        R"(C:\Users\Niklas\Code\yage\demo\SimpleWindow\assets\frag.spv)",
        platform::IFile::AccessMode::READ)->read_all();

    constexpr gl::VertexComponent vertex_layout[] = {
        {gl::VertexComponentSize::VEC2, gl::VertexComponentType::FLOAT_32},
        {gl::VertexComponentSize::VEC3, gl::VertexComponentType::FLOAT_32},
    };

    return gl::vulkan::PipelineBuilder(context)
                          .with_shaders(vert_code, frag_code)
                          .with_rasterizer(gl::PolygonMode::FILL, gl::CullMode::NONE, 1.0f)
                          .with_render_pass(context.lock()->swap_chain_render_pass())
                          .with_viewport({0, 0, 500.0f, 500.0f},
                                         {0, 0, 500, 500})
                          .with_vertex_format(gl::PrimitiveTopology::TRIANGLE_LIST, vertex_layout,
                                              gl::VertexFormat::INTERLEAVED)
                          .build();
}

int main()
{
    std::shared_ptr<platform::IWindow> window =
            std::make_shared<platform::desktop::GlfwWindow>(500, 500, "Hello Triangle",
                                                            platform::desktop::GlfwWindow::GlApi::API_VULKAN);
    const std::shared_ptr<gl::vulkan::Instance> context = std::make_shared<gl::vulkan::Instance>(std::static_pointer_cast<platform::desktop::GlfwWindow>(window));
    context->initialize();

    const auto vertex_buffer_factory = gl::vulkan::VertexBufferCreator(context);

    const gl::VertexDataInfo vertex_data_info{
        .vertex_description = {
            {gl::VertexComponentSize::VEC2, gl::VertexComponentType::FLOAT_32},
            {gl::VertexComponentSize::VEC3, gl::VertexComponentType::FLOAT_32},
        },
        .data_layout = gl::VertexDataLayout::INTERLEAVED,
    };
    constexpr std::array<const float, 15> vertex_data = {
            0.0f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    const auto vertex_buffer = vertex_buffer_factory.create(vertex_data_info, std::as_bytes(std::span{vertex_data}));

    const auto graphics_pipeline = create_pipeline(*window, context);

    gl::vulkan::Renderer renderer(context);

    window->show();
    while (!window->shouldDestroy()) {
        window->pollEvents();

        context->prepare_frame();

        renderer.begin_command_buffer();
        renderer.begin_render_pass(context->swap_chain_frame_buffer_for_frame());
        renderer.bind_pipeline(*graphics_pipeline);
        renderer.draw(*vertex_buffer);
        renderer.end_render_pass();
        renderer.end_command_buffer();

        context->present_frame();
    }

    context->flush_resources();
    return 0;
}
