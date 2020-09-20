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

