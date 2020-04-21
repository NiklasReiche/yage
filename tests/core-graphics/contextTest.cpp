#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <platform/desktop/GlfwWindow.h>
#include <graphics/graphics.h>

using ::testing::ContainerEq;

class ContextTest : public testing::Test
{
protected:
	sys::desktop::GlfwWindow window;
	std::shared_ptr<gl::IContext> context;
	
	void SetUp() override
	{
		// Context creation
		context = window.createContext(100, 100);
	}

	void TearDown() override
	{
		
	}
};

TEST_F(ContextTest, DrawableInitializationEmpty)
{
	std::shared_ptr<gl::IDrawableCreator> dCreator = context->getDrawableCreator();

	const std::vector<float> vertices;
	const std::vector<unsigned int> vertexLayout = { 1 };
	std::shared_ptr<gl::IDrawable> drawable = dCreator->createDrawable(vertices, vertexLayout, gl::VertexFormat::BATCHED);

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Drawable>(drawable)->isValid());
	ASSERT_TRUE(drawable->isEmpty());
}

TEST_F(ContextTest, DrawableInitializationWithData)
{
	std::shared_ptr<gl::IDrawableCreator> dCreator = context->getDrawableCreator();

	const std::vector<float> vertices = { 1.0f, 0.0f, -1.0, 2.5 };
	const std::vector<unsigned int> vertexLayout = { 1 };
	std::shared_ptr<gl::IDrawable> drawable = dCreator->createDrawable(vertices, vertexLayout, gl::VertexFormat::BATCHED);

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Drawable>(drawable)->isValid());
	ASSERT_FALSE(drawable->isEmpty());
}

TEST_F(ContextTest, DrawableGetSetData)
{
	std::shared_ptr<gl::IDrawableCreator> dCreator = context->getDrawableCreator();

	const std::vector<unsigned int> vertexLayout = { 1 };
	std::shared_ptr<gl::IDrawable> drawable = dCreator->createDrawable(std::vector<float>(), vertexLayout, gl::VertexFormat::BATCHED);

	const std::vector<float> vertices = { 1.0f, 0.0f, -1.0, 2.5 };
	drawable->setData(vertices);
	ASSERT_FALSE(drawable->isEmpty());

	const auto drawableData = drawable->getSubData(0, 4);
	ASSERT_THAT(drawableData, ContainerEq(vertices));
}

TEST_F(ContextTest, Texture2DInitialization)
{
	std::shared_ptr<gl::ITextureCreator> tCreator = context->getTextureCreator();

	const std::vector<unsigned char> image = {
		0, 0, 0,	1, 2, 1,
		100, 200,	1, 6, 40,
		1, 2, 3,	4, 7, 5};
	std::shared_ptr<gl::ITexture2D> texture = tCreator->createTexture2D(image, 2, 3, gl::ImageFormat::RGB);

	ASSERT_TRUE(std::static_pointer_cast<gl3::GL3_Texture2D>(texture)->isValid());

	ASSERT_EQ(2, texture->getWidth());
	ASSERT_EQ(3, texture->getHeight());
	ASSERT_EQ(3, texture->getChannels());
}

TEST_F(ContextTest, FrameInitialization)
{
	std::shared_ptr<gl::IFrameCreator> tCreator = context->getFrameCreator();

	std::shared_ptr<gl::IFrame> frame = tCreator->createFrame(90, 50, gl::ImageFormat::RGBA);

	ASSERT_EQ(90, frame->getWidth());
	ASSERT_EQ(50, frame->getHeight());
	ASSERT_EQ(4, frame->getChannels());
}