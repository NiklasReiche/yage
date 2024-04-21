#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <core/gl/color.h>
#include <font/fontHeader.h>
#include "core/platform/desktop/FileReader.h"
#include "font/converter/converter.h"

int main()
{
    std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(1000, 300,
                                                                                                "Text Renderer");
    std::shared_ptr<gl::IContext> context = gl::createContext(window);

    auto fileReader = platform::desktop::FileReader();

    /*
    std::cout << "Starting font conversion." << std::endl;
	auto converter = font::FontConverter(context->getTextureCreator());
    converter.convertFont("assets/OpenSans-Regular.ttf", "assets/OpenSans-Regular.font");
    std::cout << "Finished font conversion." << std::endl;
    */

    std::shared_ptr<font::Font> font = nullptr;
    {
        auto loader = font::FontLoader(context->getTextureCreator());
        auto fontFile = fileReader.openBinaryFile("assets/OpenSans-Regular.font", platform::IFile::AccessMode::READ);
        font = loader.loadFont(*fontFile);
    }

    auto textShader = context->getShaderCreator()->createShader(
            font::ShaderTemplate().textShader_vert,
            font::ShaderTemplate().textShader_frag);

    const std::array<float, 24> vertices = {
            -1.0f, 1.0f, 0.0f, 1.0f,    // Top Left
            -1.0f, -1.0f, 0.0f, 0.0f,    // Bottom Left
            1.0f, -1.0f, 1.0f, 0.0f,    // Bottom Right
            1.0f, 1.0f, 1.0f, 1.0f,    // Top Right
    };
    const std::array<unsigned int, 6> indices = {0, 1, 2, 2, 3, 0};
    auto quad = context->getDrawableCreator()->createDrawable(vertices, indices, {2, 2}, gl::VertexFormat::INTERLEAVED);
    const std::string vertexCode = fileReader.
            openTextFile("assets/shader.vert", platform::IFile::AccessMode::READ)->readAll();
    const std::string fragmentCode = fileReader.
            openTextFile("assets/shader.frag", platform::IFile::AccessMode::READ)->readAll();
    auto atlasShader = context->getShaderCreator()->createShader(vertexCode, fragmentCode);

    auto text = font::Text(context->getDrawableCreator(), "example text", font, gml::Vec2f(), gl::Color::BLACK, 100);

    auto renderer = context->getRenderer();
	window->show();
    renderer->setClearColor(gl::Color::WHITE);
    renderer->enableBlending();
	while (!window->shouldDestroy())
	{
		renderer->clear();

        renderer->useShader(*textShader);
        textShader->setUniform("projection", gml::matrix::orthographic<float>(0, window->getWidth(), window->getHeight(), 0, 0, 1));
        renderer->bindTexture(text.getTexture());
        renderer->draw(text.getDrawable());

        //renderer->useShader(*atlasShader);
        //renderer->bindTexture(*font->textureAtlas);
        //renderer->draw(*quad);

		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
