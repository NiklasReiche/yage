#define DESKTOP
#define GL3
#include <graphics\Graphics.h>
#include <platform\Platform.h>

#include <input\InputController.h>
#include <gui\gui.h>

class GuiTest
{
public:
	std::unique_ptr<gui::Master> master;
	gui::PushButton* button_1;

	GuiTest(platform::PlatformHandle * platform, gl::GraphicsContext * gl, input::InputController * inputController)
	{
		master = std::make_unique<gui::Master>(platform, gl, inputController);
		master->addFont("D:/Dev/Projects/YAGE/Tests/gui/res/arial.font");

		gui::W_Geometry b_metrics{ gml::Vec2<float>(10.0f), gml::Vec2<float>() };
		gui::W_Border b_border{ 1, gl::Color::BLACK };
		gui::W_Text b_text{ "Button1", "arial", 16, gl::Color::BLACK, 0x00u };

		button_1 = master->createWidget<gui::PushButton>(nullptr, b_metrics, b_border, gl::Color::CYAN, 0xAAAAAAFF);
		button_1->setCallback(std::bind(&GuiTest::on_button_1_click, this));
		button_1->setText(b_text);
	}

	void on_button_1_click()
	{
		button_1->setTextColor(gl::Color::GREY);
	}
};

int main()
{
	platform::PlatformHandle platformHandle;

	gl::GraphicsContext glContext(&platformHandle, 500, 500);
	glContext.makeCurrent();
	glContext.showWindow();

	input::InputController inputController(&platformHandle);

	input::InputListener* listener = inputController.addListener();

	GuiTest gui(&platformHandle, &glContext, &inputController);
	
	while (!glContext.getCloseFlag())
	{
		gui.master->update();

		glContext.swapBuffers();
		inputController.poll();
	}

	return 0;
}