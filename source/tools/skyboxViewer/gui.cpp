#include "gui.h"
#include "platform/Platform.h"
#include <graphics/Graphics.h>

Gui::Gui(sys::PlatformHandle* platform, gl::GraphicsContext* gl, input::InputController* input)
	: reader(img::ImageReader(platform)), platform(platform), gl(gl)
{
	master = new gui::Master(platform, gl, input);
	try {
		master->addFont("D:/DEV/Projects/yage/Tests/gui/res/arial.font");
	}
	catch (sys::FileException& exception) {
		platform->log("ERROR::FONT_LOADER: could not open file " + exception.file());
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
	frameTemplate.color = gl::Color::TRANSPARENT;
	frameTemplate.border.size = 2;
	gui::Frame* frame = master->createWidget<gui::Frame>(nullptr, frameTemplate);

	gui::ButtonTemplate buttonTemplate;
	buttonTemplate.geometry.size = gml::Vec2f(0.3, 1);
	buttonTemplate.geometry.sizeHint.x = gui::SizeHint::INFINITE;
	buttonTemplate.geometry.sizeHint.y = gui::SizeHint::ASPECT;
	buttonTemplate.geometry.offset = gml::Vec2f(0.3, 10);
	buttonTemplate.geometry.offsetHint.x = gui::OffsetHint::INFINITE;
	buttonTemplate.color = gl::Color::TRANSPARENT;
	buttonTemplate.hoverColor = gl::Color::TRANSPARENT;
	buttonTemplate.clickColor = gl::Color::TRANSPARENT;
	buttonTemplate.border.size = 1;
	buttonTemplate.command = std::bind(&Gui::on_button_click, this);
	buttonTemplate.text.text = "back";
	buttonTemplate.text.alignX = gui::TextAlignmentX::CENTER;
	buttonTemplate.text.alignY = gui::TextAlignmentY::BOTTOM;
	buttonTemplate.texture.filename = "D:/DEV/Projects/YAGE/source/tools/skyboxViewer/assets/gui/plus.png";
	buttonTemplate.hoverTexture.filename = "D:/DEV/Projects/YAGE/source/tools/skyboxViewer/assets/gui/plus_large.png";
	button = master->createWidget<gui::PushButton>(frame, buttonTemplate);

	buttonTemplate.geometry.offset.y = 0;
	buttonTemplate.command = std::bind(&Gui::on_button_click2, this);
	buttonTemplate.text.text = "top";
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
	buttonTemplate.command = std::bind(&Gui::on_button_click3, this);
	buttonTemplate.text.text = "left";
	button3 = master->createWidget<gui::PushButton>(frame1, buttonTemplate);
	buttonTemplate.command = std::bind(&Gui::on_button_click4, this);
	buttonTemplate.text.text = "front";
	button4 = master->createWidget<gui::PushButton>(frame1, buttonTemplate);
	buttonTemplate.command = std::bind(&Gui::on_button_click5, this);
	buttonTemplate.text.text = "right";
	button5 = master->createWidget<gui::PushButton>(frame1, buttonTemplate);

	buttonTemplate.geometry.offset.x = 0.3;
	buttonTemplate.geometry.size.x = 0.3;
	buttonTemplate.command = std::bind(&Gui::on_button_click6, this);
	buttonTemplate.text.text = "bottom";
	button6 = master->createWidget<gui::PushButton>(frame, buttonTemplate);

	thumbnailSize = button->getSize().x;
}

void Gui::on_button_click()
{
	std::string filename = platform->openFileDialog(lastPath);
	if (filename.size() > 0) {
		gl::Texture texture = reader.readFile(filename, img::FORCE_CHANNELS::RGBA).toTexture(gl);
		texture.resize(thumbnailSize, thumbnailSize);
		gui::WidgetTextureTemplate temp;
		temp.texture = texture;
		button->setIdleTexture(temp);
		lastPath = filename;
	}
}
void Gui::on_button_click2()
{
	std::string filename = platform->openFileDialog(lastPath);
	if (filename.size() > 0) {
		gl::Texture texture = reader.readFile(filename, img::FORCE_CHANNELS::RGBA).toTexture(gl);
		texture.resize(thumbnailSize, thumbnailSize);
		gui::WidgetTextureTemplate temp;
		temp.texture = texture;
		button2->setIdleTexture(temp);
		lastPath = filename;
	}
}
void Gui::on_button_click3()
{
	std::string filename = platform->openFileDialog(lastPath);
	if (filename.size() > 0) {
		gl::Texture texture = reader.readFile(filename, img::FORCE_CHANNELS::RGBA).toTexture(gl);
		texture.resize(thumbnailSize, thumbnailSize);
		gui::WidgetTextureTemplate temp;
		temp.texture = texture;
		button3->setIdleTexture(temp);
		lastPath = filename;
	}
}
void Gui::on_button_click4()
{
	std::string filename = platform->openFileDialog(lastPath);
	if (filename.size() > 0) {
		gl::Texture texture = reader.readFile(filename, img::FORCE_CHANNELS::RGBA).toTexture(gl);
		texture.resize(thumbnailSize, thumbnailSize);
		gui::WidgetTextureTemplate temp;
		temp.texture = texture;
		button4->setIdleTexture(temp);
		lastPath = filename;
	}
}
void Gui::on_button_click5()
{
	std::string filename = platform->openFileDialog(lastPath);
	if (filename.size() > 0) {
		gl::Texture texture = reader.readFile(filename, img::FORCE_CHANNELS::RGBA).toTexture(gl);
		texture.resize(thumbnailSize, thumbnailSize);
		gui::WidgetTextureTemplate temp;
		temp.texture = texture;
		button5->setIdleTexture(temp);
		lastPath = filename;
	}
}
void Gui::on_button_click6()
{
	std::string filename = platform->openFileDialog(lastPath);
	if (filename.size() > 0) {
		gl::Texture texture = reader.readFile(filename, img::FORCE_CHANNELS::RGBA).toTexture(gl);
		texture.resize(thumbnailSize, thumbnailSize);
		gui::WidgetTextureTemplate temp;
		temp.texture = texture;
		button6->setIdleTexture(temp);
		lastPath = filename;
	}
}