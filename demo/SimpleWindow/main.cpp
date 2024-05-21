#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <core/gl/color.h>
#include <core/platform/desktop/FileReader.h>

#include <core/gl/vulkan/Instance.h>

using namespace yage;

int main()
{
    std::shared_ptr<platform::IWindow> window =
            std::make_shared<platform::desktop::GlfwWindow>(500, 500, "Hello Triangle",
                                                            platform::desktop::GlfwWindow::GlApi::API_VULKAN);
    std::shared_ptr<gl::vulkan::Instance> context = std::make_shared<gl::vulkan::Instance>();

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

    window->show();
    while (!window->shouldDestroy()) {
#if 0
        renderer->clear();
        renderer->draw(*triangle);
        window->swapBuffers();
#endif

        window->pollEvents();
    }

    return 0;
}
