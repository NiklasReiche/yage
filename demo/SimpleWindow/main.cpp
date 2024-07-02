#include <memory>

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>

#include <core/gl/vulkan/Instance.h>
#include <core/gl/vulkan/Renderer.h>

using namespace yage;

std::unique_ptr<gl::vulkan::Pipeline> create_pipeline(platform::IWindow& window, const std::weak_ptr<gl::vulkan::Instance>& context)
{
    const auto file_reader = window.getFileReader();
    auto vert_code = file_reader->openBinaryFile(
        R"(C:\Users\Niklas\Code\yage\demo\SimpleWindow\assets\vert.spv)",
        platform::IFile::AccessMode::READ)->read_all();
    auto frag_code = file_reader->openBinaryFile(
        R"(C:\Users\Niklas\Code\yage\demo\SimpleWindow\assets\frag.spv)",
        platform::IFile::AccessMode::READ)->read_all();
#if 0
    constexpr gl::VertexComponent vertex_layout[] = {
        {gl::VertexComponentSize::VEC2, gl::VertexComponentType::FLOAT_32},
        {gl::VertexComponentSize::VEC3, gl::VertexComponentType::FLOAT_32},
    };
#endif

    std::vector<gl::VertexComponent> tmp;

    return gl::vulkan::PipelineBuilder(context)
                          .with_shaders(vert_code, frag_code)
                          .with_rasterizer(gl::PolygonMode::FILL, gl::CullMode::NONE, 1.0f)
                          .with_render_pass(context.lock()->swap_chain_render_pass())
                          .with_viewport({0, 0, 500.0f, 500.0f},
                                         {0, 0, 500, 500})
                          .with_vertex_format(gl::PrimitiveTopology::TRIANGLE_LIST, tmp,
                                              gl::VertexFormat::INTERLEAVED)
                          .build();
}

int main()
{
    std::shared_ptr<platform::IWindow> window =
            std::make_shared<platform::desktop::GlfwWindow>(500, 500, "Hello Triangle",
                                                            platform::desktop::GlfwWindow::GlApi::API_VULKAN);
    std::shared_ptr<gl::vulkan::Instance> context = std::make_shared<gl::vulkan::Instance>(std::static_pointer_cast<platform::desktop::GlfwWindow>(window));
    context->initialize();

#if 0
    const std::array<float, 15> vertices = {
            -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    const std::array<unsigned int, 3> indices = {0, 1, 2};
    const std::array<unsigned int, 2> vertex_layout = {2, 3};
    auto triangle = context->getDrawableCreator()->createDrawable(
            vertices, indices, vertex_layout, gl::VertexFormat::INTERLEAVED);

    auto fileReader = platform::desktop::FileReader();
    const std::string vertexCode = fileReader.openTextFile("assets/shader.vert",
                                                           platform::IFile::AccessMode::READ)->readAll();
    const std::string fragmentCode = fileReader.openTextFile("assets/shader.frag",
                                                             platform::IFile::AccessMode::READ)->readAll();
    auto shader = context->getShaderCreator()->createShader(vertexCode, fragmentCode);

    auto renderer = context->getRenderer();
    renderer->setClearColor(gl::Color::BLACK);
    renderer->useShader(*shader);
#endif

    const auto graphics_pipeline = create_pipeline(*window, context);

    gl::vulkan::Renderer renderer(context);

    window->show();
    while (!window->shouldDestroy()) {
        window->pollEvents();
#if 0
        renderer->clear();
        renderer->draw(*triangle);
        window->swapBuffers();
#endif

        context->prepare_frame();

        renderer.begin_command_buffer();
        renderer.begin_render_pass(context->swap_chain_frame_buffer_for_frame());
        renderer.bind_pipeline(*graphics_pipeline);
        renderer.draw();
        renderer.end_render_pass();
        renderer.end_command_buffer();

        context->present_frame();
    }

    context->flush_resources();
    return 0;
}
