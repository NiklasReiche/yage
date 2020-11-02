#include <catch2/catch.hpp>

#include <core/platform/desktop/GlfwWindow.h>

TEST_CASE("Window Test")
{
	SECTION("Dimensions") {
		SECTION("valid") {
			std::shared_ptr<platform::IWindow> window = std::make_shared<platform::desktop::GlfwWindow>(200, 100);
			CHECK(100 == window->getHeight());
			CHECK(200 == window->getWidth());

		} SECTION("invalid") {
			SECTION("width zero") {
				CHECK_THROWS_AS(std::make_shared<platform::desktop::GlfwWindow>(0, 100), std::invalid_argument);
			} SECTION("height zero") {
				CHECK_THROWS_AS(std::make_shared<platform::desktop::GlfwWindow>(100, 0), std::invalid_argument);
			} SECTION("width and height zero") {
				CHECK_THROWS_AS(std::make_shared<platform::desktop::GlfwWindow>(0, 0), std::invalid_argument);
			} SECTION("width negative") {
				CHECK_THROWS_AS(std::make_shared<platform::desktop::GlfwWindow>(-10, 100), std::invalid_argument);
			} SECTION("height negative") {
				CHECK_THROWS_AS(std::make_shared<platform::desktop::GlfwWindow>(100, -10), std::invalid_argument);
			} SECTION("width and height negative") {
				CHECK_THROWS_AS(std::make_shared<platform::desktop::GlfwWindow>(-10, -10), std::invalid_argument);
			}
		}
	}
}
