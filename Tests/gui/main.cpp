#define DESKTOP
#define GL3
#include <graphics/Graphics.h>
#include <platform/Platform.h>

#include <input/InputController.h>
#include <gui/gui.h>
#include <gui/Animation.h>

class GuiTest
{
private:
	platform::PlatformHandle* platform;
	gl::GraphicsContext* gl;
	input::InputController* input;

public:
	gui::Master* master;

	gui::Frame* frame_1;
	gui::Frame* frame_2;

	gui::PushButton* button_1;
	int clicks = 0;
	gui::Label* label_1;

	gui::CheckButton* button_2;
	gui::Label* label_2;

	gui::Frame* radioFrame;
	gui::RadioGroup<int>* radio;
	gui::Label* label_3;

	gui::Animation* animation_1;

	GuiTest(platform::PlatformHandle * platform, gl::GraphicsContext * gl, input::InputController * inputController)
		: platform(platform), gl(gl), input(inputController)
	{
		master = new gui::Master(platform, gl, inputController);
		try {
			master->addFont("C:/Users/Niklas/Desktop/DEV/Projects/yage/Tests/gui/res/arial.font");
		}
		catch (FileException& exception) {
			platform->log("ERROR::FONT_LOADER: could not open file " + exception.what());
		}

		gui::FrameTemplate frameTemplate;
		frameTemplate.color = gl::Color::GREY;
		frameTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		frameTemplate.parentSizeHint.x = gui::ParentSizeHint::WRAP_AROUND;
		frameTemplate.parentSizeHint.y = gui::ParentSizeHint::WRAP_AROUND;
		frameTemplate.layoutMargin = gml::Vec2f(5.0f);
		frameTemplate.geometry.anchor = gui::Anchor::TOP_LEFT;
		frameTemplate.geometry.size = gml::Vec2<float>(100, 100);
		frameTemplate.geometry.offset = gml::Vec2<float>(50, 50);

		frame_1 = master->createWidget<gui::Frame>(nullptr, frameTemplate);

		frameTemplate.layoutType = gui::LayoutType::H_LIST_LAYOUT;
		frameTemplate.geometry.offset = gml::Vec2<float>(0.0f, 0.0f);
		frame_2 = master->createWidget<gui::Frame>(frame_1, frameTemplate);

		gui::LabelTemplate labelLayout;
		labelLayout.border.size = 1;
		labelLayout.geometry.offset = gml::Vec2<float>(5.0f);
		gui::TextLayout textLayout;
		textLayout.text = "Label";

		label_1 = master->createWidget<gui::Label>(frame_2, labelLayout, textLayout);
		master->createWidget<gui::Label>(frame_2, labelLayout, textLayout);
		master->createWidget<gui::Label>(frame_2, labelLayout, textLayout);

		gui::ButtonTemplate buttonLayout;
		buttonLayout.geometry.anchor = gui::Anchor::TOP_RIGHT;
		buttonLayout.geometry.offset = gml::Vec2<float>(10);
		buttonLayout.hoverColor = 0xDDDDDDFF;
		buttonLayout.clickColor = gl::Color::GREY;
		buttonLayout.border.size = 1;
		buttonLayout.text.text = "Push Button";
		buttonLayout.command = std::bind(&GuiTest::on_button_1_click, this);

		button_1 = master->createWidget<gui::PushButton>(frame_1, buttonLayout);

		animation_1 = frame_1->createAnimation(master, frame_1->getPosition(), frame_1->getPosition() + gml::Vec2<float>(200, 0), 2);

#if 0
		buttonLayout.geometry.position = gml::Vec2<float>(10.0f, 100.0f);
		buttonLayout.text.text = "Check Button";
		buttonLayout.command = std::bind(&GuiTest::on_button_2_click, this);

		button_2 = master->createWidget<gui::CheckButton>(nullptr, buttonLayout);

		gui::WidgetLayout labelLayout;
		labelLayout.geometry.position = gml::Vec2<float>(15.0f + button_1->getSize().x, 10.0f);
		gui::TextLayout textLayout;
		textLayout.text = "clicks: 0";

		label_1 = master->createWidget<gui::Label>(frame_1, labelLayout, textLayout);

		labelLayout.geometry.position = gml::Vec2<float>(16.0f + button_2->getSize().x, 100.0f);
		textLayout.text = "state: unselected";

		label_2 = master->createWidget<gui::Label>(nullptr, labelLayout, textLayout);
#endif


		gui::FrameTemplate radioFrameTemplate;
		radioFrameTemplate.geometry.anchor = gui::Anchor::TOP_LEFT;
		radioFrameTemplate.geometry.offset = gml::Vec2<float>(400, 50);
		radioFrameTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		radioFrameTemplate.parentSizeHint.x = gui::ParentSizeHint::WRAP_AROUND;
		radioFrameTemplate.parentSizeHint.y = gui::ParentSizeHint::WRAP_AROUND;
		radioFrameTemplate.color = gl::Color::CYAN;

		radioFrame = master->createWidget<gui::Frame>(nullptr, radioFrameTemplate);

		gui::LabelTemplate radiolabelTemplate;
		radiolabelTemplate.border.size = 1;
		labelLayout.geometry.offset = gml::Vec2<float>(5.0f, 5.0f);
		gui::TextLayout radiotextLayout;
		radiotextLayout.text = "state: 1";

		label_3 = master->createWidget<gui::Label>(radioFrame, radiolabelTemplate, radiotextLayout);

		gui::FrameTemplate radioTemplate;
		radioTemplate.geometry.offset = gml::Vec2<float>(0.0f, 5.0f);
		radioTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
		gui::ButtonTemplate radioButtonLayout;
		radioButtonLayout.border.size = 1;
		radioButtonLayout.hoverColor = 0xDDDDDDFF;
		radioButtonLayout.clickColor = gl::Color::GREY;
		radioButtonLayout.command = std::bind(&GuiTest::on_radio_click, this);

		radio = master->createWidget<gui::RadioGroup<int>>(radioFrame, radioTemplate, radioButtonLayout, 0);
		radio->addButton("Radio Button 1", 1, true);
		radio->addButton("Radio Button 2", 2);
		radio->addButton("Radio Button 3", 3);

		
	}
	~GuiTest()
	{
		delete master;
	}

	void on_button_1_click()
	{
		animation_1->start();
		/*
		clicks++;
		gui::TextLayout textLayout;
		textLayout.text = "clicks: " + clib::to_string(clicks);
		label_1->setText(textLayout);
		*/
	}
	void on_button_2_click()
	{
		gui::TextLayout textLayout;
		if (button_2->getState()) {
			textLayout.text = "state: selected";
		}
		else {
			textLayout.text = "state: unselected";
		}
		label_2->setText(textLayout);
	}
	void on_radio_click()
	{
		gui::TextLayout textLayout;
		textLayout.text = "state: " + clib::to_string(radio->getState());
		label_3->setText(textLayout);
	}
};

int main()
{
	platform::PlatformHandle platformHandle;
	gl::GraphicsContext glContext(&platformHandle, 1000, 500);
	input::InputController inputController(&platformHandle);

	GuiTest guiTest(&platformHandle, &glContext, &inputController);

	
	glContext.showWindow();
	platformHandle.getTimeStep();
	while (!glContext.getCloseFlag())
	{
		double dt = platformHandle.getTimeStep();

		guiTest.master->update(dt);

		glContext.swapBuffers();
		inputController.poll();
	}

	return 0;
}