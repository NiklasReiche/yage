#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <core/gl/color.h>
#include <core/platform/desktop/FileReader.h>

using namespace yage;

int main()
{
	std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(500, 500, "Simple Window");
	std::shared_ptr<gl::IContext> context = gl::createContext(window);


	const std::array<float, 15> vertices = {
	    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
	    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	    0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
	};
	const std::array<unsigned int, 3> indices = { 0, 1, 2 };
	auto triangle = context->getDrawableCreator()->createDrawable(vertices, indices, std::vector<unsigned int>{ 2, 3 }, gl::VertexFormat::INTERLEAVED);

	auto fileReader = platform::desktop::FileReader();
	const std::string vertexCode = fileReader.
		openTextFile("assets/shader.vert", platform::IFile::AccessMode::READ)->readAll();
	const std::string fragmentCode = fileReader.
		openTextFile("assets/shader.frag", platform::IFile::AccessMode::READ)->readAll();
	auto shader = context->getShaderCreator()->createShader(vertexCode, fragmentCode);

	auto renderer = context->getRenderer();
	renderer->setClearColor(gl::Color::BLACK);
	renderer->useShader(*shader);

	window->show();
	while (!window->shouldDestroy())
	{
		renderer->clear();
		renderer->draw(*triangle);

		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
