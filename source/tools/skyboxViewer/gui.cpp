#include "gui.h"
#include "platform/platform.h"
#include <graphics/graphics.h>


Gui::Gui(sys::PlatformHandle* platform, gl::GraphicsContext* gl, input::InputController* input)
	: reader(img::ImageReader(platform)), platform(platform), gl(gl)
{
	master = new gui::Master(platform, gl, input);
	try {
		master->addFont(installPath + "/assets/fonts/arial.font");
	}
	catch (sys::FileException& exception) {
		platform->log("ERROR::FONT_LOADER: could not open file " + exception.file());
		platform->log(util::to_string(exception.error()));
	}

	gui::FrameTemplate frameTemplate;
	frameTemplate.geometry.size = gml::Vec2f(0.3, 1);
	frameTemplate.geometry.sizeHint = gml::Vec2<gui::SizeHint>(gui::SizeHint::INFINITE);
	frameTemplate.layoutType = gui::LayoutType::V_LIST_LAYOUT;
	frameTemplate.color = gl::Color::TRANSPARENT;
	frameTemplate.border.size = 2;
	gui::Frame* frame = master->createWidget<gui::Frame>(nullptr, frameTemplate);

	gui::FrameTemplate frameTemplate1;
	frameTemplate1.geometry.size = gml::Vec2f(1, 0.3 * 0.3);
	frameTemplate1.geometry.sizeHint.x = gui::SizeHint::INFINITE;
	frameTemplate1.geometry.sizeHint.y = gui::SizeHint::INFINITE;
	frameTemplate1.layoutType = gui::LayoutType::H_LIST_LAYOUT;
	frameTemplate1.layoutMargin = gml::Vec2f(10, 10);
	frameTemplate1.color = gl::Color::TRANSPARENT;
	gui::Frame* frame1 = master->createWidget<gui::Frame>(frame, frameTemplate1);

	gui::FrameTemplate frameTemplate2;
	frameTemplate2.geometry.size = gml::Vec2f(1, 0.3 * 0.3);
	frameTemplate2.geometry.sizeHint.x = gui::SizeHint::INFINITE;
	frameTemplate2.geometry.sizeHint.y = gui::SizeHint::INFINITE;
	frameTemplate2.layoutType = gui::LayoutType::H_LIST_LAYOUT;
	frameTemplate2.layoutMargin = gml::Vec2f(10, 10);
	frameTemplate2.color = gl::Color::TRANSPARENT;
	gui::Frame* frame2 = master->createWidget<gui::Frame>(frame, frameTemplate2);

	gui::FrameTemplate frameTemplate3;
	frameTemplate3.geometry.size = gml::Vec2f(1, 0.3 * 0.3);
	frameTemplate3.geometry.sizeHint.x = gui::SizeHint::INFINITE;
	frameTemplate3.geometry.sizeHint.y = gui::SizeHint::INFINITE;
	frameTemplate3.layoutType = gui::LayoutType::H_LIST_LAYOUT;
	frameTemplate3.layoutMargin = gml::Vec2f(10, 10);
	frameTemplate3.color = gl::Color::TRANSPARENT;
	gui::Frame* frame3 = master->createWidget<gui::Frame>(frame, frameTemplate3);


	gui::ButtonTemplate buttonTemplate;
	buttonTemplate.geometry.size = gml::Vec2f(1, 1);
	buttonTemplate.geometry.sizeHint.x = gui::SizeHint::ASPECT;
	buttonTemplate.geometry.sizeHint.y = gui::SizeHint::INFINITE;
	buttonTemplate.geometry.offset = gml::Vec2f(0, 0);
	buttonTemplate.geometry.offsetHint.x = gui::OffsetHint::INFINITE;
	buttonTemplate.border.size = 1;
	buttonTemplate.color = gl::Color::TRANSPARENT;
	buttonTemplate.hoverColor = gl::Color::TRANSPARENT;
	buttonTemplate.clickColor = gl::Color::TRANSPARENT;
	buttonTemplate.text.text = "";
	buttonTemplate.text.size = 14;
	buttonTemplate.text.alignX = gui::TextAlignmentX::CENTER;
	buttonTemplate.text.alignY = gui::TextAlignmentY::BOTTOM;
	buttonTemplate.texture.filename = installPath + "/assets/gui/plus.png";
	buttonTemplate.hoverTexture.filename = installPath + "/assets/gui/plus_large.png";
	gui::PushButton* buttonDummy = master->createWidget<gui::PushButton>(frame1, buttonTemplate);
	buttonDummy->hide();

	buttonTemplate.text.text = "top";
	buttonTemplate.command = std::bind(&Gui::on_button_top_click, this);
	button_top = master->createWidget<gui::PushButton>(frame1, buttonTemplate);

	buttonTemplate.text.text = "left";
	buttonTemplate.command = std::bind(&Gui::on_button_left_click, this);
	button_left = master->createWidget<gui::PushButton>(frame2, buttonTemplate);

	buttonTemplate.text.text = "front";
	buttonTemplate.command = std::bind(&Gui::on_button_front_click, this);
	button_front = master->createWidget<gui::PushButton>(frame2, buttonTemplate);

	buttonTemplate.text.text = "right";
	buttonTemplate.command = std::bind(&Gui::on_button_right_click, this);
	button_right = master->createWidget<gui::PushButton>(frame2, buttonTemplate);

	buttonTemplate.text.text = "back";
	buttonTemplate.command = std::bind(&Gui::on_button_back_click, this);
	button_back = master->createWidget<gui::PushButton>(frame2, buttonTemplate);

	buttonTemplate.text.text = "";
	buttonDummy = master->createWidget<gui::PushButton>(frame3, buttonTemplate);
	buttonDummy->hide();

	buttonTemplate.text.text = "bottom";
	buttonTemplate.command = std::bind(&Gui::on_button_bottom_click, this);
	button_bottom = master->createWidget<gui::PushButton>(frame3, buttonTemplate);

	thumbnailSize = button_top->getSize().x;


	gui::ButtonTemplate buttonTemplateLoad;
	buttonTemplateLoad.geometry.sizeHint.x = gui::SizeHint::FIXED;
	buttonTemplateLoad.geometry.sizeHint.y = gui::SizeHint::FIXED;
	buttonTemplateLoad.geometry.offset = gml::Vec2f(0.3, 50);
	buttonTemplateLoad.geometry.offsetHint.x = gui::OffsetHint::INFINITE;
	buttonTemplateLoad.border.size = 1;
	buttonTemplateLoad.color = gl::Color::WHITE;
	buttonTemplateLoad.hoverColor = gl::Color::GREY;
	buttonTemplateLoad.clickColor = gl::Color::GREY;
	buttonTemplateLoad.text.text = "load";
	buttonTemplateLoad.text.alignX = gui::TextAlignmentX::CENTER;
	buttonTemplateLoad.text.alignY = gui::TextAlignmentY::CENTER;
	buttonTemplateLoad.command = std::bind(&Gui::on_button_load_click, this);
	master->createWidget<gui::PushButton>(frame, buttonTemplateLoad);


	gui::LabelTemplate labelTemplate;
	labelTemplate.color = gl::Color::TRANSPARENT;
	labelTemplate.geometry.anchor = gui::Anchor::BOTTOM_LEFT;
	labelTemplate.geometry.offset.y = 10;
	labelTemplate.text.size = 20;
	labelTemplate.text.text = "press X to toggle view mode";
	tooltipLabel = master->createWidget<gui::Label>(frame, labelTemplate);
	tooltipLabel->hide();
}

void Gui::setTexture(gui::PushButton* button, std::string filename)
{
	gl::Texture texture = reader.readFile(filename, img::FORCE_CHANNELS::RGBA).toTexture(gl);
	texture.resize(thumbnailSize, thumbnailSize);
	gui::WidgetTextureTemplate temp;
	temp.texture = texture;
	button->setIdleTexture(temp);
}

void Gui::on_button_load_click()
{
	if (loadCallback) {
		std::array<std::string, 6> filenames = {
			path_right,
			path_left,
			path_top,
			path_bottom,
			path_back,
			path_front
		};
		loadCallback(filenames);
	}
}

void Gui::on_button_top_click()
{
	std::string filename = platform->openFileDialog(lastPath, {"*.png"}, "Image Files (png, bmp)");
	if (filename.size() > 0) {
		setTexture(button_top, filename);
		path_top = filename;
		lastPath = filename;
	}
}
void Gui::on_button_left_click()
{
	std::string filename = platform->openFileDialog(lastPath, {"*.png"}, "Image Files (png, bmp)");
	if (filename.size() > 0) {
		setTexture(button_left, filename);
		path_left = filename;
		lastPath = filename;
	}
}
void Gui::on_button_front_click()
{
	std::string filename = platform->openFileDialog(lastPath, {"*.png"}, "Image Files (png, bmp)");
	if (filename.size() > 0) {
		setTexture(button_front, filename);
		path_front = filename;
		lastPath = filename;
	}
}
void Gui::on_button_right_click()
{
	std::string filename = platform->openFileDialog(lastPath, {"*.png"}, "Image Files (png, bmp)");
	if (filename.size() > 0) {
		setTexture(button_right, filename);
		path_right = filename;
		lastPath = filename;
	}
}
void Gui::on_button_back_click()
{
	std::string filename = platform->openFileDialog(lastPath, {"*.png"}, "Image Files (png, bmp)");
	if (filename.size() > 0) {
		setTexture(button_back, filename);
		path_back = filename;
		lastPath = filename;
	}
}
void Gui::on_button_bottom_click()
{
	std::string filename = platform->openFileDialog(lastPath, {"*.png"}, "Image Files (png, bmp)");
	if (filename.size() > 0) {
		setTexture(button_bottom, filename);
		path_bottom = filename;
		lastPath = filename;
	}
}
