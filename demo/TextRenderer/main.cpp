#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <core/gl/color.h>
#include <font/fontHeader.h>
#include "core/platform/desktop/FileReader.h"
#include "font/converter/converter.h"

int main()
{
    std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(1000, 800,
                                                                                                "Text Renderer");
    std::shared_ptr<gl::IContext> context = gl::createContext(window);

    auto fileReader = platform::desktop::FileReader();

#if 0
    std::cout << "Starting font conversion." << std::endl;
	auto converter = font::FontConverter(context);
    converter.convert("assets/OpenSans-Regular.ttf", "assets/OpenSans-Regular.font");
    std::cout << "Finished font conversion." << std::endl;
#endif

    std::shared_ptr<font::Font> font;
    {
        auto loader = font::FontLoader(context->getTextureCreator());
        auto fontFile = fileReader.openBinaryFile("assets/OpenSans-Regular.font", platform::IFile::AccessMode::READ);
        font = loader.loadFont(*fontFile);
        font->textureAtlas->configTextureFilter(gl::TextureFilter::LINEAR, gl::TextureFilter::LINEAR);
    }

    const std::string vertexCodeText = fileReader.
            openTextFile("assets/text.vert", platform::IFile::AccessMode::READ)->readAll();
    const std::string fragmentCodeText = fileReader.
            openTextFile("assets/text.frag", platform::IFile::AccessMode::READ)->readAll();
    auto textShader = context->getShaderCreator()->createShader(vertexCodeText, fragmentCodeText);

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

    auto text = font::Text(context->getDrawableCreator(), "Example Text 123!", font, gml::Vec3f(0, 0, -1), gl::Color::BLACK, 400);
    auto text2 = font::Text(context->getDrawableCreator(), "Example Text 123!", font, gml::Vec3f(0, 400, -1),
                           gl::Color::BLACK, 100);
    auto text3 = font::Text(context->getDrawableCreator(), "Example Text 123!", font, gml::Vec3f(0, 500, -1),
                            gl::Color::BLACK, 50);

    auto renderer = context->getRenderer();
	window->show();
    renderer->setClearColor(gl::Color::WHITE);
	while (!window->shouldDestroy())
	{
		renderer->clear();

        renderer->setViewport(0, 0, window->getPixelWidth(), window->getPixelHeight());
        renderer->useShader(*textShader);
        textShader->setUniform("projection",
                               gml::matrix::orthographic<float>(0, window->getPixelWidth(), window->getPixelHeight(), 0, 0.1, 100));
        renderer->bindTexture(text.getTexture());
        renderer->enableBlending();
        renderer->draw(text.getDrawable());
        renderer->draw(text2.getDrawable());
        renderer->draw(text3.getDrawable());
        //renderer->disableBlending();
        //renderer->draw(text2.getDrawable());

        //renderer->useShader(*atlasShader);
        //renderer->bindTexture(*font->textureAtlas);
        //renderer->draw(*quad);

		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
