#include <graphics/Graphics.h>
#include <platform/Platform.h>

#include <input/InputController.h>
#include <gui/gui.h>
#include <gui/Animation.h>


platform::PlatformHandle* platformHandle;
gl::GraphicsContext* glContext;
input::InputController* inputController;

gui::Master* master;

gui::Label* fpsCounter;
gui::Frame* frame_1;

void displayFPS(double dt);

int main()
{
	platformHandle = new platform::PlatformHandle();
	glContext = new gl::GraphicsContext(platformHandle, 1000, 500);
	inputController = new input::InputController(platformHandle);

	master = new gui::Master(platformHandle, glContext, inputController);
	try {
		master->addFont("C:/Users/Niklas/Desktop/DEV/Projects/yage/Tests/gui/res/arial.font");
	}
	catch (FileException& exception) {
		platformHandle->log("ERROR::FONT_LOADER: could not open file " + exception.what());
	}


	gui::LabelTemplate fpslabelTemplate;
	fpslabelTemplate.color = gl::Color::TRANSPARENT;
	fpslabelTemplate.text.size = 14;
	fpsCounter = master->createWidget<gui::Label>(nullptr, fpslabelTemplate);

	gui::FrameTemplate frameTemplate;
	frameTemplate.geometry.offset = gml::Vec2f(10, 50);
	frameTemplate.geometry.size = gml::Vec2f(0.5f, 0.5f);
	frameTemplate.sizeHint.x = gui::SizeHint::INFINITE;
	frameTemplate.sizeHint.y = gui::SizeHint::INFINITE;
	frameTemplate.color = gl::Color::LIGHT_BLUE;
	frameTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
	frameTemplate.layoutMargin = gml::Vec2f(5);
	frame_1 = master->createWidget<gui::Frame>(nullptr, frameTemplate);

	gui::LabelTemplate label2Template;
	label2Template.border.size = 1;
	label2Template.text.text = "Label";
	label2Template.offsetHint.x = gui::OffsetHint::INFINITE;
	label2Template.geometry.offset.x = 0.5f;
	label2Template.text.size = 20;
	master->createWidget<gui::Label>(frame_1, label2Template);


	gui::FrameTemplate frame2Template;
	frame2Template.color = gl::Color::ORANGE;
	frame2Template.layoutType = gui::LayoutType::H_LIST_LAYOUT;
	frame2Template.layoutMargin = gml::Vec2f(5);
	gui::Frame* frame_2 = master->createWidget<gui::Frame>(frame_1, frame2Template);

	gui::LabelTemplate labelTemplate;
	labelTemplate.border.size = 1;
	labelTemplate.text.text = "Label";
	master->createWidget<gui::Label>(frame_2, labelTemplate);

	gui::ButtonTemplate buttonTemplate;
	buttonTemplate.border.size = 1;
	buttonTemplate.hoverColor = gl::Color::GREY;
	buttonTemplate.text.text = "Button";
	master->createWidget<gui::PushButton>(frame_2, buttonTemplate);


	glContext->disableVSync();
	glContext->showWindow();
	platformHandle->getTimeStep();
	while (!glContext->getCloseFlag())
	{
		double dt = platformHandle->getTimeStep();

		displayFPS(dt);

		master->update(dt);
		glContext->swapBuffers();
		inputController->poll();
	}

	return 0;
}

void displayFPS(double dt)
{
	double frameRate = 1.0 / dt;
	fpsCounter->setText(clib::to_string((int)frameRate));
}