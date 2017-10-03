#include "gui.h"
#include "platform/Platform.h"

Gui::Gui(sys::PlatformHandle* platform, gl::GraphicsContext* gl, input::InputController* input)
	: platform(platform), gl(gl)
{
	master = new gui::Master(platform, gl, input);
	try {
		master->addFont("D:/DEV/Projects/yage/Tests/gui/res/arial.font");
	}
	catch (FileException& exception) {
		platform->log("ERROR::FONT_LOADER: could not open file " + exception.path());
	}

	gui::LabelTemplate labelTemplate;
	labelTemplate.color = gl::Color::TRANSPARENT;
	labelTemplate.text.size = 20;
	labelTemplate.text.text = "Skybox";
	//master->createWidget<gui::Label>(nullptr, labelTemplate);

	gui::FrameTemplate frameTemplate;
	frameTemplate.geometry.size = gml::Vec2f(0.5, 1);
	frameTemplate.geometry.sizeHint = gml::Vec2<gui::SizeHint>(gui::SizeHint::INFINITE);
	frameTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
	frameTemplate.color = gl::Color::LIGHT_BLUE;
	gui::Frame* frame = master->createWidget<gui::Frame>(nullptr, frameTemplate);

	gui::ButtonTemplate buttonTemplate;
	buttonTemplate.geometry.size = gml::Vec2f(0.3, 1);
	buttonTemplate.geometry.sizeHint.x = gui::SizeHint::INFINITE;
	buttonTemplate.geometry.sizeHint.y = gui::SizeHint::ASPECT;
	buttonTemplate.geometry.offset = gml::Vec2f(0.3, 10);
	buttonTemplate.geometry.offsetHint.x = gui::OffsetHint::INFINITE;
	buttonTemplate.color = gl::Color::TRANSPARENT;
	unsigned int color = gl::Color::GREY;
	gl::setAlpha(color, 200);
	buttonTemplate.hoverColor = color;
	buttonTemplate.clickColor = gl::Color::TRANSPARENT;
	buttonTemplate.border.size = 1;
	buttonTemplate.command = std::bind(&Gui::on_button_click, this);
	buttonTemplate.texture = "D:/DEV/Projects/YAGE/Tests/skyboxeditor/plus.png";
	button = master->createWidget<gui::PushButton>(frame, buttonTemplate);

	buttonTemplate.geometry.offset.y = 0;
	button2 = master->createWidget<gui::PushButton>(frame, buttonTemplate);

	gui::FrameTemplate frameTemplate1;
	frameTemplate1.geometry.size = gml::Vec2f(0.3*3, (1.0f/3.0f));
	frameTemplate1.geometry.sizeHint.x = gui::SizeHint::INFINITE;
	frameTemplate1.geometry.sizeHint.y = gui::SizeHint::ASPECT;
	frameTemplate1.layoutType = gui::LayoutType::H_LIST_LAYOUT;
	frameTemplate1.color = gl::Color::TRANSPARENT;
	gui::Frame* frame1 = master->createWidget<gui::Frame>(frame, frameTemplate1);

	buttonTemplate.geometry.offset = gml::Vec2f(0, 0);
	buttonTemplate.geometry.size.x = 1.0f/3.0f;
	button3 = master->createWidget<gui::PushButton>(frame1, buttonTemplate);
	button4 = master->createWidget<gui::PushButton>(frame1, buttonTemplate);
	button5 = master->createWidget<gui::PushButton>(frame1, buttonTemplate);

	buttonTemplate.geometry.offset.x = 0.3;
	buttonTemplate.geometry.size.x = 0.3;
	button6 = master->createWidget<gui::PushButton>(frame, buttonTemplate);
}

void Gui::on_button_click()
{
	std::string filename = sys::openFileDialog(lastPath);
	if (filename.size() > 0) {
		button->setTexture(filename);
		lastPath = filename;
	}
}
void Gui::on_button_click2()
{
	std::string filename = sys::openFileDialog(lastPath);
	if (filename.size() > 0) {
		button2->setTexture(filename);
		lastPath = filename;
	}
}
void Gui::on_button_click3()
{
	std::string filename = sys::openFileDialog(lastPath);
	if (filename.size() > 0) {
		button3->setTexture(filename);
		lastPath = filename;
	}
}
void Gui::on_button_click4()
{
	std::string filename = sys::openFileDialog(lastPath);
	if (filename.size() > 0) {
		button4->setTexture(filename);
		lastPath = filename;
	}
}
void Gui::on_button_click5()
{
	std::string filename = sys::openFileDialog(lastPath);
	if (filename.size() > 0) {
		button5->setTexture(filename);
		lastPath = filename;
	}
}
void Gui::on_button_click6()
{
	std::string filename = sys::openFileDialog(lastPath);
	if (filename.size() > 0) {
		button6->setTexture(filename);
		lastPath = filename;
	}
}