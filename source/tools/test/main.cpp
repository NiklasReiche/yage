#include <gui/gui.h>

int main()
{
  sys::PlatformHandle* platform = new sys::PlatformHandle();
	gl::GraphicsContext* glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Skybox Viewer");

  std::cout << glGetString(GL_VERSION) << std::endl;

  input::InputController* inputController = new input::InputController(platform);
	input::InputListener* inputListener = inputController->addListener();

  gui::Master* master = new gui::Master(platform, glContext, inputController);
	try {
		//master->addFont("/home/niklas/dev/yage/source/tools/skyboxViewer/assets/fonts/arial.font");
		master->addFont("D:/dev/projects/yage/source/tools/skyboxViewer/assets/fonts/arial.font");
	}
	catch (sys::FileException& exception) {
		platform->log("ERROR::FONT_LOADER: could not open file " + exception.file());
		platform->log(util::to_string(exception.error()));
	}

  gui::FrameTemplate frameTemplate4;
  frameTemplate4.geometry.anchor = gui::Anchor::TOP_LEFT;
  frameTemplate4.border.size = 1;
  frameTemplate4.color = gl::Color::TRANSPARENT;
  frameTemplate4.layoutType = gui::LayoutType::V_LIST_LAYOUT;
  frameTemplate4.layoutMargin = gml::Vec2f(5.0f);

  gui::Frame* frame_4 = master->createWidget<gui::Frame>(nullptr, frameTemplate4);

  gui::LabelTemplate labelTemplateButtons;
  labelTemplateButtons.text.text = "Button showcase";
  labelTemplateButtons.text.size = 20;

  master->createWidget<gui::Label>(frame_4, labelTemplateButtons);


  gui::FrameTemplate frameTemplateButtons;
  frameTemplateButtons.border.size = 1;
  frameTemplateButtons.color = gl::Color::TRANSPARENT;
  frameTemplateButtons.layoutType = gui::LayoutType::H_LIST_LAYOUT;
  frameTemplateButtons.layoutMargin = gml::Vec2f(5.0f);

  gui::Frame* frame_buttons = master->createWidget<gui::Frame>(frame_4, frameTemplateButtons);


  gui::FrameTemplate frameTemplateClicks;
  frameTemplateClicks.color = gl::Color::TRANSPARENT;
  frameTemplateClicks.layoutType = gui::LayoutType::V_LIST_LAYOUT;

  gui::Frame* frame_clicks = master->createWidget<gui::Frame>(frame_buttons, frameTemplateClicks);


  gui::LabelTemplate labelTemplateClicks;
  labelTemplateClicks.text.text = "clicks: 0";

  gui::Label* label_clicks = master->createWidget<gui::Label>(frame_clicks, labelTemplateClicks);

  gui::ButtonTemplate buttonTemplateClicks;
  buttonTemplateClicks.geometry.offset = gml::Vec2<float>(0, 5);
  buttonTemplateClicks.color = gl::Color::BLUE;
  buttonTemplateClicks.hoverColor = 0xDDDDDDFFu;
  buttonTemplateClicks.clickColor = gl::Color::BLUE;
  buttonTemplateClicks.border.size = 1;
  buttonTemplateClicks.text.text = "Push Button";
  buttonTemplateClicks.text.alignY = gui::TextAlignmentY::CENTER;
  buttonTemplateClicks.text.alignX = gui::TextAlignmentX::CENTER;
  //buttonTemplateClicks.command = std::bind(&GuiTest::on_button_1_click, this);

  gui::PushButton* button_clicks = master->createWidget<gui::PushButton>(frame_clicks, buttonTemplateClicks);

  glContext->showWindow();
	platform->getTimeStep();
	while (!platform->shouldDestroy())
	{
		double dt = platform->getTimeStep();

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    master->update(dt);

		glContext->swapBuffers();
		inputController->poll();
	}

  delete glContext;
  delete platform;
  return 0;
}
