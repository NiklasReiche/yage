#include <catch2/catch.hpp>

#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/graphics.h>
#include <core/gl/opengl/Context.h>


TEST_CASE("Context Test")
{
	std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(100, 100);
	std::shared_ptr<gl::IContext> context = gl::createContext(window);


	SECTION("FrameInitialization") {
		std::shared_ptr<gl::IFrameCreator> tCreator = context->getFrameCreator();

		std::shared_ptr<gl::IFrame> frame = tCreator->createFrame(90, 50, gl::ImageFormat::RGBA);

		CHECK(90 == frame->getWidth());
		CHECK(50 == frame->getHeight());
		CHECK(4 == frame->getChannels());
	}
}
