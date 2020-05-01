#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <platform/desktop/GlfwWindow.h>
#include <graphics/graphics.h>

using ::testing::ContainerEq;

class TextureTest : public testing::Test
{
protected:
	sys::desktop::GlfwWindow window;
	std::shared_ptr<gl::IContext> context;
	std::shared_ptr<gl::ITextureCreator> tCreator;

	void SetUp() override
	{
		// Context creation
		context = window.createContext(100, 100);
		tCreator = context->getTextureCreator();
	}

	void TearDown() override
	{

	}
};

/*
 * Texture2D Initialization
 */

TEST_F(TextureTest, Initialize_WithData)
{
	const std::vector<unsigned char> image = {
		0, 0, 0, 1, 2, 1,
		1, 0, 20, 1, 6, 40,
		1, 2, 3, 4, 7, 5 };
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(2, 3, gl::ImageFormat::RGB, image);

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Texture2D>(texture)->isValid());

	ASSERT_EQ(2, texture->getWidth());
	ASSERT_EQ(3, texture->getHeight());
	ASSERT_EQ(3, texture->getChannels());
	ASSERT_EQ(gl::ImageFormat::RGB, texture->getFormat());
}

TEST_F(TextureTest, Initialize_WithDataFormatted)
{
	const std::vector<unsigned char> image = {
		0, 0, 0, 1, 2, 1,
		1, 0, 20, 1, 6, 40,
		1, 2, 3, 4, 7, 5 };
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(3, 3, gl::ImageFormat::RG, image);

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Texture2D>(texture)->isValid());

	ASSERT_EQ(3, texture->getWidth());
	ASSERT_EQ(3, texture->getHeight());
	ASSERT_EQ(2, texture->getChannels());
	ASSERT_EQ(gl::ImageFormat::RG, texture->getFormat());
}

TEST_F(TextureTest, Initialize_Empty)
{
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(2, 3, gl::ImageFormat::RGB, std::vector<unsigned char>());

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Texture2D>(texture)->isValid());

	ASSERT_EQ(2, texture->getWidth());
	ASSERT_EQ(3, texture->getHeight());
	ASSERT_EQ(3, texture->getChannels());
	ASSERT_EQ(gl::ImageFormat::RGB, texture->getFormat());
}

TEST_F(TextureTest, Initialize_EmptyFormatted)
{
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(2, 3, gl::ImageFormat::RGB, std::vector<unsigned char>(),
		                          { gl::ImageFormat::RG, gl::RowAlignment::B_2 });

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Texture2D>(texture)->isValid());

	ASSERT_EQ(2, texture->getWidth());
	ASSERT_EQ(3, texture->getHeight());
	ASSERT_EQ(3, texture->getChannels());
	ASSERT_EQ(gl::ImageFormat::RGB, texture->getFormat());
}

TEST_F(TextureTest, Initialize_WrongDataSize)
{
	// data too small
	ASSERT_THROW(
		tCreator->createTexture2D(2, 3, gl::ImageFormat::RGB, { 1, 2, 3 }),
		std::invalid_argument);

	// data too big
	ASSERT_THROW(
		tCreator->createTexture2D(1, 2, gl::ImageFormat::R, { 1, 2, 3, 4 }),
		std::invalid_argument);
}

TEST_F(TextureTest, Initialize_NonPositiveDimensions)
{
	// width negative
	ASSERT_THROW(
		tCreator->createTexture2D(-2, 3, gl::ImageFormat::RGBA, std::vector<unsigned char>()),
		std::invalid_argument);

	// height negative
	ASSERT_THROW(
		tCreator->createTexture2D(2, -3, gl::ImageFormat::RGBA, std::vector<unsigned char>()),
		std::invalid_argument);

	// width and height negative
	ASSERT_THROW(
		tCreator->createTexture2D(-2, -3, gl::ImageFormat::RGBA, std::vector<unsigned char>()),
		std::invalid_argument);

	// width zero
	ASSERT_THROW(
		tCreator->createTexture2D(0, 3, gl::ImageFormat::RGBA, std::vector<unsigned char>()),
		std::invalid_argument);

	// height zero
	ASSERT_THROW(
		tCreator->createTexture2D(2, 0, gl::ImageFormat::RGBA, std::vector<unsigned char>()),
		std::invalid_argument);

	// width and height zero
	ASSERT_THROW(
		tCreator->createTexture2D(0, 0, gl::ImageFormat::RGBA, std::vector<unsigned char>()),
		std::invalid_argument);
}

TEST_F(TextureTest, Initialize_UndefinedFormat)
{
	ASSERT_THROW(
		tCreator->createTexture2D(2, 3, gl::ImageFormat::UNDEFINED, std::vector<unsigned char>()),
		std::invalid_argument);
}

TEST_F(TextureTest, GetImage)
{
	const std::vector<unsigned char> image = {
		0, 0, 0, 1, 2, 1,
		10, 1, 2, 1, 6, 40,
		1, 2, 3, 4, 7, 5,
		0, 5, 1, 8, 9, 3 };
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(2, 4, gl::ImageFormat::RGB, image);

	const auto data = texture->getImage();
	ASSERT_THAT(data, ContainerEq(image));
}

TEST_F(TextureTest, SetImage)
{
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(2, 4, gl::ImageFormat::RGB, std::vector<unsigned char>());

	const std::vector<unsigned char> image = {
		0, 0, 0, 1, 2, 1,
		10, 1, 2, 1, 6, 40,
		1, 2, 3, 4, 7, 5,
		0, 5, 1, 8, 9, 3 };
	texture->setImage(image);

	const auto data = texture->getImage();
	ASSERT_THAT(data, ContainerEq(image));
}

TEST_F(TextureTest, SetSubImage)
{
	const std::vector<unsigned char> image = {
		0, 0, 0,  1, 2, 1,
		10, 1, 2, 1, 6, 40,
		1, 2, 3,  4, 7, 5,
		0, 5, 1,  8, 9, 3 };
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(2, 4, gl::ImageFormat::RGB, image);

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Texture2D>(texture)->isValid());

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
	ASSERT_THAT(data, ContainerEq(newImage));
}

TEST_F(TextureTest, Mipmap)
{
	const std::vector<unsigned char> image = {
		0, 0, 0, 1, 2, 1,
		10, 1, 2, 1, 6, 40,
		1, 2, 3, 4, 7, 5,
		0, 5, 1, 8, 9, 3 };
	std::shared_ptr<gl::ITexture2D> texture =
		tCreator->createTexture2D(2, 4, gl::ImageFormat::RGB, image);

	texture->generateMipmaps();

	auto data = texture->getMipmapImage(0);
	ASSERT_EQ(24, data.size());
	data = texture->getMipmapImage(1);
	ASSERT_EQ(6, data.size());
	data = texture->getMipmapImage(2);
	ASSERT_EQ(3, data.size());
}

#pragma clang diagnostic pop