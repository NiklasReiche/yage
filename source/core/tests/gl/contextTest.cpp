#include <catch2/catch_all.hpp>

#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/graphics.h>
#include <core/gl/opengl/Context.h>


TEST_CASE("Context Test")
{
	std::shared_ptr<yage::platform::IWindow> window = std::make_shared<yage::platform::desktop::GlfwWindow>(100, 100);
	std::shared_ptr<yage::gl::IContext> context = yage::gl::createContext(window);


	SECTION("FrameInitialization") {
		std::shared_ptr<yage::gl::IFrameCreator> tCreator = context->getFrameCreator();

		std::shared_ptr<yage::gl::IFrame> frame = tCreator->createFrame(90, 50, yage::gl::ImageFormat::RGBA);

		CHECK(90 == frame->getWidth());
		CHECK(50 == frame->getHeight());
		CHECK(4 == frame->getChannels());
	}
}
