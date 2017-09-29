#include "gui.h"

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
	labelTemplate.texture = "D:/DEV/projects/yage/tests/skyboxeditor/test.png";

	master->createWidget<gui::Label>(nullptr, labelTemplate);

	gui::WidgetTemplate widgetTemplate;
	widgetTemplate.geometry.size = gml::Vec2f(500, 500);
	widgetTemplate.geometry.offset = gml::Vec2f(0, 100);
	//widgetTemplate.border.size = 5;
	widgetTemplate.texture = "D:/DEV/projects/yage/tests/skyboxeditor/test2.png";
	gui::Widget* widget = master->createWidget<gui::Widget>(nullptr, widgetTemplate);

	//widget->texCoords = gml::Vec4f(0, 0, 0.5f, 0.5f);
	//widget->updateParams();
}