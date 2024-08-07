#include <catch2/catch_all.hpp>

#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/graphics.h>
#include <core/gl/opengl/Context.h>


TEST_CASE("Drawable Test")
{
	std::shared_ptr<yage::platform::IWindow> window = std::make_shared<yage::platform::desktop::GlfwWindow>(100, 100);
	std::shared_ptr<yage::gl::IContext> context = yage::gl::createContext(window);
	std::shared_ptr<yage::gl::IDrawableCreator> dCreator = context->getDrawableCreator();

	SECTION("DrawableInitializationEmpty") {
		std::shared_ptr<yage::gl::IDrawable> drawable =
			dCreator->createDrawable({ }, { 1 }, yage::gl::VertexFormat::BATCHED);

		CHECK(drawable->isEmpty());
	}

	SECTION("DrawableInitializationWithData") {
		const std::vector<float> vertices = { 1.0f, 0.0f, -1.0, 2.5 };
		std::shared_ptr<yage::gl::IDrawable> drawable =
			dCreator->createDrawable(vertices, { 1 }, yage::gl::VertexFormat::BATCHED);

		CHECK(!drawable->isEmpty());
	}

	SECTION("DrawableGetSetData") {
		std::shared_ptr<yage::gl::IDrawable> drawable =
			dCreator->createDrawable({ }, { 1 }, yage::gl::VertexFormat::BATCHED);

		CHECK(drawable->isEmpty());

		const std::vector<float> vertices = { 1.0f, 0.0f, -1.0, 2.5 };
		drawable->setData(vertices);

		CHECK(!drawable->isEmpty());

		const auto drawableData = drawable->getSubData(0, 4);
		CHECK(drawableData == vertices);
	}
}
