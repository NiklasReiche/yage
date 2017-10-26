#include <gui/gui.h>

int main()
{
  sys::PlatformHandle* platform = new sys::PlatformHandle();
	gl::GraphicsContext* glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Skybox Viewer");

  input::InputController* inputController = new input::InputController(platform);
	input::InputListener* inputListener = inputController->addListener();

  gui::Master* master = new gui::Master(platform, glContext, inputController);
	try {
		master->addFont("/home/niklas/dev/yage/source/tools/skyboxViewer/assets/fonts/arial.font");
	}
	catch (sys::FileException& exception) {
		platform->log("ERROR::FONT_LOADER: could not open file " + exception.file());
		platform->log(util::to_string(exception.error()));
	}

  gui::LabelTemplate labelTemp;
  labelTemp.color = gl::Color::BLUE;
  labelTemp.text.text = "TEST";
  gui::Label* label = master->createWidget<gui::Label>(nullptr, labelTemp);

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
