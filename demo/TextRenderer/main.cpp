#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <core/gl/color.h>
#include <font/fontHeader.h>
#include "core/platform/desktop/FileReader.h"
#include "font/converter.h"
#include "font/shaders.h"

int main(int argc, char *argv[], char *[])
{
    if (argc < 2) {
        std::cout << "Missing path to font file" << std::endl;
        return 1;
    }
    std::vector<std::string> args;
    args.assign(argv, argv + argc);

    std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(1200, 800,
                                                                                                "Text Renderer");
    std::shared_ptr<gl::IContext> context = gl::createContext(window);

    auto fileReader = platform::desktop::FileReader();

    std::string fontPath = args[1];

    if (std::find(args.begin(), args.end(), "-generate") != args.end()) {
        std::vector<font::Codepoint> additionalCharacters = {
                0x00c4, // Ä
                0x00d6, // Ö
                0x00dc, // Ü
                0x00e4, // ä
                0x00f6, // ö
                0x00fc  // ü
        };
        auto codepoints = font::FontConverter::codepoint_set_ascii();
        codepoints.insert(std::end(codepoints), std::begin(additionalCharacters), std::end(additionalCharacters));
        std::cout << "Starting font conversion." << std::endl;
        auto converter = font::FontConverter(context);
        converter.convert(fontPath + ".ttf",fontPath + ".font",
                          codepoints, 512, 4, 4, 64);
        std::cout << "Finished font conversion." << std::endl;
    }

    res::Store<font::Font> fontStore;
    font::FontFileLoader fontLoader(context->getTextureCreator(), window->getFileReader());
    res::Resource<font::Font> font = fontStore.loadResource(fontLoader, fontPath + ".font");

    auto textShader = context->getShaderCreator()->createShader(font::shaders::TextShader::vert,
                                                                font::shaders::TextShader::frag);

    std::u32string string = U"ÜÄÖ The quick brown fox jumps over the lazy dog.";
    auto text100 = font::Text(context->getDrawableCreator(), string, font,
                              {.color = gl::Color::BLACK, .pt_size = 100});
    auto text50 = font::Text(context->getDrawableCreator(), string, font,
                             {.color = gl::Color::BLACK, .pt_size = 50});
    auto text24 = font::Text(context->getDrawableCreator(), string, font,
                             {.color = gl::Color::BLACK, .pt_size = 24});
    auto text16 = font::Text(context->getDrawableCreator(), string, font,
                             {.color = gl::Color::BLACK, .pt_size = 16});
    auto text12 = font::Text(context->getDrawableCreator(), string, font,
                             {.color = gl::Color::BLACK, .pt_size = 12});
    auto text11 = font::Text(context->getDrawableCreator(), string, font,
                             {.color = gl::Color::BLACK, .pt_size = 11});
    auto text9 = font::Text(context->getDrawableCreator(), string, font,
                            {.color = gl::Color::BLACK, .pt_size = 9});

    text100.update_color(gl::Color::BLUE);

    auto renderer = context->getRenderer();
	window->show();
    renderer->setClearColor(gl::Color::WHITE);
    renderer->enableBlending();
    int step = 0;
	while (!window->shouldDestroy())
	{
		renderer->clear();

        renderer->setViewport(0, 0, window->getPixelWidth(), window->getPixelHeight());
        auto projection = gml::matrix::orthographic<float>(0, window->getPixelWidth(),
                                                           window->getPixelHeight(), 0,
                                                           0.1, 10000);

        renderer->useShader(*textShader);
        textShader->setUniform("spread", 1.0f);
        renderer->bindTexture(*font.get().textureAtlas);

        float yPos = 0;
        textShader->setUniform("projection", projection);
        textShader->setUniform("scale", 100.0f / 16.0f);
        renderer->draw(text100.drawable());
        yPos += 150;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("scale", 50.0f / 16.0f);
        renderer->draw(text50.drawable());
        yPos += 90;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("scale", 24.0f / 16.0f);
        renderer->draw(text24.drawable());
        yPos += 50;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("scale", 16.0f / 16.0f);
        renderer->draw(text16.drawable());
        yPos += 40;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("scale", 12.0f / 16.0f);
        renderer->draw(text12.drawable());
        yPos += 30;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("scale", 11.0f / 16.0f);
        renderer->draw(text11.drawable());
        yPos += 30;

        textShader->setUniform("projection", projection * gml::matrix::translate<float>(0, yPos, 0));
        textShader->setUniform("scale", 9.0f / 16.0f);
        renderer->draw(text9.drawable());
        yPos += 30;

        textShader->setUniform("projection",
                               projection *
                               gml::matrix::translate<float>(700, yPos + 100, -1000) *
                               gml::matrix::axisAngle(gml::Vec3f(0, 1, 1), gml::to_rad((float) ((step) % 360))) *
                               gml::matrix::translate<float>(-250, -20, 0)
                               );
        step++;
        textShader->setUniform("scale", 16.0f / 16.0f);
        renderer->draw(text16.drawable());

		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
