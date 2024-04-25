#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <core/gl/color.h>
#include <font/fontHeader.h>
#include "core/platform/desktop/FileReader.h"
#include "font/converter.h"

int main()
{
    std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(1200, 500,
                                                                                                "Text Renderer");
    std::shared_ptr<gl::IContext> context = gl::createContext(window);

    auto fileReader = platform::desktop::FileReader();

    std::vector<unsigned long> characters = {
            0x0938094d,
    };

#if 0
    std::cout << "Starting font conversion." << std::endl;
	auto converter = font::FontConverter(context);
    converter.convert("assets/OpenSans-Regular.ttf", "assets/OpenSans-Regular.font", 512, 4, 4, 64);
    std::cout << "Finished font conversion." << std::endl;
#endif

    std::shared_ptr<font::Font> font;
    {
        auto loader = font::FontLoader(context->getTextureCreator());
        auto fontFile = fileReader.openBinaryFile("assets/OpenSans-Regular.font", platform::IFile::AccessMode::READ);
        font = loader.loadFont(*fontFile);
    }

    const std::string vertexCodeText = fileReader.
            openTextFile("assets/text.vert", platform::IFile::AccessMode::READ)->readAll();
    const std::string fragmentCodeText = fileReader.
            openTextFile("assets/text.frag", platform::IFile::AccessMode::READ)->readAll();
    auto textShader = context->getShaderCreator()->createShader(vertexCodeText, fragmentCodeText);

    auto string = "The quick brown fox jumps over the lazy dog.";
    auto text100 = font::Text(context->getDrawableCreator(), string, font,
                           gl::Color::BLACK, 100);
    auto text50 = font::Text(context->getDrawableCreator(), string, font,
                           gl::Color::BLACK, 50);
    auto text24 = font::Text(context->getDrawableCreator(), string, font,
                            gl::Color::BLACK, 24);
    auto text16 = font::Text(context->getDrawableCreator(), string, font,
                            gl::Color::BLACK, 16);
    auto text12 = font::Text(context->getDrawableCreator(), string, font,
                             gl::Color::BLACK, 12);
    auto text11 = font::Text(context->getDrawableCreator(), string, font,
                             gl::Color::BLACK, 11);
    auto text9 = font::Text(context->getDrawableCreator(), string, font,
                             gl::Color::BLACK, 9);

    auto renderer = context->getRenderer();
	window->show();
    renderer->setClearColor(gl::Color::WHITE);
    renderer->enableBlending();
	while (!window->shouldDestroy())
	{
		renderer->clear();

        renderer->setViewport(0, 0, window->getPixelWidth(), window->getPixelHeight());
        auto projection = gml::matrix::orthographic<float>(0, window->getPixelWidth(),
                                                           window->getPixelHeight(), 0,
                                                           0.1, 100);

        renderer->useShader(*textShader);
        renderer->bindTexture(*font->textureAtlas);

        float yPos = 0;
        textShader->setUniform("projection", projection);
        textShader->setUniform("smoothing2", 0.05f);
        renderer->draw(text100.getDrawable());
        yPos += 150;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("smoothing2", 0.07f);
        renderer->draw(text50.getDrawable());
        yPos += 90;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("smoothing2", 0.15f);
        renderer->draw(text24.getDrawable());
        yPos += 50;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("smoothing2", 0.25f);
        renderer->draw(text16.getDrawable());
        yPos += 40;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("smoothing2", 0.3f);
        renderer->draw(text12.getDrawable());
        yPos += 30;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("smoothing2", 0.3f);
        renderer->draw(text11.getDrawable());
        yPos += 30;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("smoothing2", 0.4f);
        renderer->draw(text9.getDrawable());

		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
