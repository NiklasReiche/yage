#include <catch2/catch_all.hpp>

#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/graphics.h>


TEST_CASE("Texture Test")
{
	std::shared_ptr<yage::platform::IWindow> window = std::make_shared<yage::platform::desktop::GlfwWindow>(100, 100);
	std::shared_ptr<yage::gl::IContext> context = yage::gl::createContext(window);
	std::shared_ptr<yage::gl::ITextureCreator> tCreator = context->getTextureCreator();

	SECTION("Initialization") {
		SECTION("with data") {
			const std::vector<unsigned char> image = {
				0, 0, 0,    1, 2, 1,
				1, 0, 20,   1, 6, 40,
				1, 2, 3,    4, 7, 5 };

			SECTION("without format conversion") {
				std::shared_ptr<yage::gl::ITexture2D> texture = tCreator->createTexture2D(2, 3, yage::gl::ImageFormat::RGB, image);

				CHECK(2 == texture->getWidth());
				CHECK(3 == texture->getHeight());
				CHECK(3 == texture->getChannels());
				CHECK(yage::gl::ImageFormat::RGB == texture->getFormat());
			}

			SECTION("with format conversion") {
				std::shared_ptr<yage::gl::ITexture2D> texture = tCreator->createTexture2D(3, 3, yage::gl::ImageFormat::RG, image);

				CHECK(3 == texture->getWidth());
				CHECK(3 == texture->getHeight());
				CHECK(2 == texture->getChannels());
				CHECK(yage::gl::ImageFormat::RG == texture->getFormat());
			}
		}

		SECTION("without data") {
			SECTION("without format conversion") {
				std::shared_ptr<yage::gl::ITexture2D> texture =
					tCreator->createTexture2D(2, 3, yage::gl::ImageFormat::RGB, std::vector<unsigned char>());

				CHECK(2 == texture->getWidth());
				CHECK(3 == texture->getHeight());
				CHECK(3 == texture->getChannels());
				CHECK(yage::gl::ImageFormat::RGB == texture->getFormat());
			}

			SECTION("with format conversion") {
				std::shared_ptr<yage::gl::ITexture2D> texture =
					tCreator->createTexture2D(2, 3, yage::gl::ImageFormat::RGB, std::vector<unsigned char>(),
					                          { yage::gl::ImageFormat::RG, yage::gl::RowAlignment::B_2 });

				CHECK(2 == texture->getWidth());
				CHECK(3 == texture->getHeight());
				CHECK(3 == texture->getChannels());
				CHECK(yage::gl::ImageFormat::RGB == texture->getFormat());
			}
		}

		SECTION("wrong data size") {
			SECTION("data too small") {
				CHECK_THROWS_AS(
					tCreator->createTexture2D(2, 3, yage::gl::ImageFormat::RGB, std::vector<unsigned char>{ 1, 2, 3 }),
					std::invalid_argument);
			}

			SECTION("data too large") {
				CHECK_THROWS_AS(
					tCreator->createTexture2D(1, 2, yage::gl::ImageFormat::R, std::vector<unsigned char>{ 1, 2, 3, 4 }),
					std::invalid_argument);
			}
		}

		SECTION("undefined format") {
			CHECK_THROWS_AS(
				tCreator->createTexture2D(2, 3, yage::gl::ImageFormat::UNDEFINED, std::vector<unsigned char>()),
				std::invalid_argument);
		}

		SECTION("non-positive dimensions") {
			// width negative
			CHECK_THROWS_AS(
				tCreator->createTexture2D(-2, 3, yage::gl::ImageFormat::RGBA, std::vector<unsigned char>()),
				std::invalid_argument);

			// height negative
			CHECK_THROWS_AS(
				tCreator->createTexture2D(2, -3, yage::gl::ImageFormat::RGBA, std::vector<unsigned char>()),
				std::invalid_argument);

			// width and height negative
			CHECK_THROWS_AS(
				tCreator->createTexture2D(-2, -3, yage::gl::ImageFormat::RGBA, std::vector<unsigned char>()),
				std::invalid_argument);

			// width zero
			CHECK_THROWS_AS(
				tCreator->createTexture2D(0, 3, yage::gl::ImageFormat::RGBA, std::vector<unsigned char>()),
				std::invalid_argument);

			// height zero
			CHECK_THROWS_AS(
				tCreator->createTexture2D(2, 0, yage::gl::ImageFormat::RGBA, std::vector<unsigned char>()),
				std::invalid_argument);

			// width and height zero
			CHECK_THROWS_AS(
				tCreator->createTexture2D(0, 0, yage::gl::ImageFormat::RGBA, std::vector<unsigned char>()),
				std::invalid_argument);
		}
	}

	SECTION("set image") {
		SECTION("set image") {
			std::shared_ptr<yage::gl::ITexture2D> texture =
				tCreator->createTexture2D(2, 4, yage::gl::ImageFormat::RGB, std::vector<unsigned char>());

			const std::vector<unsigned char> image = {
				0, 0, 0, 1, 2, 1,
				10, 1, 2, 1, 6, 40,
				1, 2, 3, 4, 7, 5,
				0, 5, 1, 8, 9, 3 };
			texture->setImage(image);

			const auto data = texture->getImage();
			CHECK(data == image);
		}

		SECTION("set sub image") {
			const std::vector<unsigned char> image = {
				0, 0, 0, 1, 2, 1,
				10, 1, 2, 1, 6, 40,
				1, 2, 3, 4, 7, 5,
				0, 5, 1, 8, 9, 3 };
			std::shared_ptr<yage::gl::ITexture2D> texture =
				tCreator->createTexture2D(2, 4, yage::gl::ImageFormat::RGB, image);

			const std::vector<unsigned char> subImage = {
				0, 1, 3, 1, 4, 10,
				1, 5, 9, 4, 8, 5 };
			texture->setSubImage({ 0, 1, 2, 2 }, subImage);

			const std::vector<unsigned char> newImage = {
				0, 0, 0, 1, 2, 1,
				0, 1, 3, 1, 4, 10,
				1, 5, 9, 4, 8, 5,
				0, 5, 1, 8, 9, 3 };

			const auto data = texture->getImage();
			CHECK(data == newImage);
		}
	}

	SECTION("mipmaps") {
		const std::vector<unsigned char> image = {
			0, 0, 0, 1, 2, 1,
			10, 1, 2, 1, 6, 40,
			1, 2, 3, 4, 7, 5,
			0, 5, 1, 8, 9, 3 };
		std::shared_ptr<yage::gl::ITexture2D> texture =
			tCreator->createTexture2D(2, 4, yage::gl::ImageFormat::RGB, image);

		texture->generateMipmaps();

		auto data = texture->getMipmapImage(0);
		CHECK(24 == data.size());
		data = texture->getMipmapImage(1);
		CHECK(6 == data.size());
		data = texture->getMipmapImage(2);
		CHECK(3 == data.size());
	}
}
