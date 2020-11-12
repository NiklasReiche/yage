#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <image/img.h>
#include <core/platform/desktop/FileReader.h>

int main(int argc, char* argv[], char*[])
{
	if (argc < 2) {
		return 1;
	}

	img::Image image;
	{
		platform::desktop::FileReader fileReader;
		auto file = fileReader.openBinaryFile(argv[1], platform::IFile::AccessMode::READ);
		image = img::readFromFile(*file, img::FORCE_CHANNELS::RGB);
	}

	std::shared_ptr<platform::IWindow> window =
		std::make_shared<platform::desktop::GlfwWindow>(image.getWidth(), image.getHeight(),"ImageViewer");
	std::shared_ptr<gl::IContext> context = gl::createContext(window);


	const std::array<float, 24> vertices = {
		-1.0f, 1.0f, 0.0f, 1.0f,    // Top Left
		-1.0f, -1.0f, 0.0f, 0.0f,    // Bottom Left
		1.0f, -1.0f, 1.0f, 0.0f,    // Bottom Right

		1.0f, -1.0f, 1.0f, 0.0f,    // Bottom Right
		1.0f, 1.0f, 1.0f, 1.0f,    // Top Right
		-1.0f, 1.0f, 0.0f, 1.0f    // Top Left
	};
	auto quad = context->getDrawableCreator()->createDrawable(vertices, { 2, 2 }, gl::VertexFormat::INTERLEAVED);


	auto texture = context->getTextureCreator()->
		createTexture2D(image.getWidth(), image.getHeight(), gl::ImageFormat::RGB, {image.data(), image.getSize()});


	const std::string vertexCode =
		"	#version 330 core											    \n"
		"	layout (location = 0) in vec2 position;						    \n"
		"	layout (location = 1) in vec2 texCoords;					    \n"
		"	out vec2 TexCoords;											    \n"
		"	void main() {												    \n"
		"		gl_Position = vec4(position.x, -position.y, 0.0f, 1.0f);    \n"
		"		TexCoords = vec2(texCoords.x, texCoords.y);				    \n"
		"	}															    \n";
	const std::string fragmentCode =
		"	#version 330 core								\n"
		"	in vec2 TexCoords;								\n"
		"	out vec4 color;									\n"
		"	uniform sampler2D screenTexture;				\n"
		"	void main() {									\n"
		"		color = texture(screenTexture, TexCoords);  \n"
		"	}											    \n";
	auto shader = context->getShaderCreator()->createShader(vertexCode, fragmentCode);

	auto renderer = context->getRenderer();
	renderer->useShader(*shader);

	window->show();
	while (!window->shouldDestroy()) {
		renderer->clear();
		renderer->bindTexture(*texture);
		renderer->draw(*quad);

		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
