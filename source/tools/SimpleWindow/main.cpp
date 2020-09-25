#include <platform/Window.h>
#include <platform/desktop/GlfwWindow.h>
#include <gl/Context.h>

int main()
{
	std::shared_ptr<sys::IWindow> window = std::make_shared<sys::desktop::GlfwWindow>(500, 500, "Simple Window");
	std::shared_ptr<gl::IContext> context = gl::createContext(window);


	std::array<float, 9> vertices = {
	    -0.5f, -0.5f, 0.0f,
	    0.5f, -0.5f, 0.0f,
	    0.0f, 0.5f, 0.0f
	};
	auto triangle = context->getDrawableCreator()->createDrawable(vertices, std::vector<unsigned int>{3}, gl::VertexFormat::BATCHED);

	const std::string vertexShaderSource =
	    "#version 330 core\n"
	    "layout (location = 0) in vec3 aPos;\n"
	    "void main()\n"
	    "{\n"
	    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	    "}";
	const std::string fragmentShaderSource =
	    "#version 330 core\n"
	    "out vec4 FragColor;\n"
	    "void main()\n"
	    "{\n"
	    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	    "}";
	auto shader = context->getShaderCreator()->createShader(vertexShaderSource, fragmentShaderSource);

	auto renderer = context->getRenderer();
	renderer->useShader(*shader);

	window->show();
	while (!window->shouldDestroy())
	{
		renderer->draw(*triangle);

		window->swapBuffers();
		window->pollEvents();
	}

	return 0;
}
