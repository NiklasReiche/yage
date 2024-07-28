#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <image/img.h>
#include <core/platform/desktop/FileReader.h>

using namespace yage;

int main(int argc, char* argv[], char*[])
{
	if (argc < 2) {
		return 1;
	}

	img::Image image;
	{
		platform::desktop::FileReader fileReader;
		auto file = fileReader.open_binary_file(argv[1], platform::IFile::AccessMode::READ);
		image = img::readFromFile(*file, img::FORCE_CHANNELS::RGB);
	}

	std::shared_ptr<platform::IWindow> window =
		std::make_shared<platform::desktop::GlfwWindow>(image.getWidth(), image.getHeight(),"ImageViewer");
	std::shared_ptr<gl::IContext> context = gl::createOpenGlContext(window);


	const std::array<float, 24> vertices = {
		-1.0f, 1.0f, 0.0f, 1.0f,    // Top Left
		-1.0f, -1.0f, 0.0f, 0.0f,    // Bottom Left
		1.0f, -1.0f, 1.0f, 0.0f,    // Bottom Right
		1.0f, 1.0f, 1.0f, 1.0f,    // Top Right
	};
	const std::array<unsigned int, 6> indices = { 0, 1, 2, 2, 3, 0 };
	auto quad = context->getDrawableCreator()->createDrawable(vertices, indices, std::vector<unsigned int>{ 2, 2 }, gl::VertexFormat::INTERLEAVED);


	auto texture = context->getTextureCreator()->
		createTexture2D(image.getWidth(), image.getHeight(), gl::ImageFormat::RGB, {image.data(), image.getSize()});

	auto fileReader = platform::desktop::FileReader();
	const std::string vertexCode = fileReader.
		open_text_file("assets/shader.vert", platform::IFile::AccessMode::READ)->read_all();
	const std::string fragmentCode = fileReader.
		open_text_file("assets/shader.frag", platform::IFile::AccessMode::READ)->read_all();
	auto shader = context->getShaderCreator()->createShader(vertexCode, fragmentCode);

	auto renderer = context->getRenderer();
	renderer->useShader(*shader);

	window->show();
	while (!window->shouldDestroy()) {
		renderer->clear();
		renderer->bindTexture(*texture);
		renderer->draw(*quad);

		window->swapBuffers();
		window->poll_events();
	}

	return 0;
}
