#include "app.h"
#include <iomanip>

App::App()
{
	platform = new sys::PlatformHandle();
	glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Skybox Viewer");
	inputController = new input::InputController(platform);
	inputListener = inputController->addListener();

	gui = new Gui(platform, glContext, inputController);

	guiViewport = gl::Viewport(0, 0, 1500, 900);
	skyboxViewport = gl::Viewport(500, 0, 1000, 900);

	gui->setLoadCallback(std::bind(&App::loadSkybox, this, std::placeholders::_1));

	inputListener->setOnMousePosEvent(std::bind(&App::on_mouse_pos_event, this, std::placeholders::_1, std::placeholders::_2));
	inputListener->setOnKeyEvent(std::bind(&App::on_key_event, this, std::placeholders::_1, std::placeholders::_2));

	camera = gl3d::Camera(gml::Vec3d(0.0, 0.0, 5.0), gml::Quatd::axisAngle(gml::Vec3d(0.0, 0.0, -1.0), 0.0));
}
App::~App()
{
	delete gui;

	delete inputController;
	delete glContext;
	delete platform;
}

int App::run()
{
	glContext->showWindow();
	platform->getTimeStep();
	while (!platform->shouldDestroy())
	{
		double dt = platform->getTimeStep();

		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glContext->setActiveViewport(guiViewport);
		gui->update(dt);

		if (isLoaded) {
			drawSkybox();
		}

		glContext->swapBuffers();
		inputController->poll();
	}

	return 0;
}

void App::loadSkybox(std::array<std::string, 6> filenames)
{
	skybox = gl3d::SkyboxLoader(platform, glContext).loadSkybox(filenames, 100, 2048);
	skybox.shader.setUniform("skybox", 0);
	gml::Mat4d proj = gml::Mat4d::perspective(45.0, (double)(skyboxViewport.x + skyboxViewport.width)/ (double)(skyboxViewport.y + skyboxViewport.height), 0.1, 1000.0);
	skybox.shader.setUniform("projection", proj);
	isLoaded = true;
	gui->showTooltip();
}
void App::drawSkybox()
{
	skybox.shader.setUniform("view", camera.getViewMatrix());
	glContext->setActiveViewport(skyboxViewport);
	glContext->useShader(skybox.shader);
	glContext->bindTexture(skybox.cubemap);
	glContext->draw(skybox.drawable);
}

void App::on_mouse_pos_event(float x, float y)
{
	if (isLoaded && mouse.isHidden) {
		gml::Vec2f dist = mouse.pos - gml::Vec2f(x, y);
		gml::Vec2f angle = dist * mouse.sensitivity;
		camera.rotateYaw(angle.x);
		camera.rotatePitch(angle.y);
	}
	mouse.pos = gml::Vec2f(x, y);
}

void App::on_key_event(input::KeyCode code, input::KeyAction action)
{
	if (code == input::KeyCode::KEY_X && action == input::KeyAction::PRESS) {
		if (mouse.isHidden) {
			platform->showCursor();
			mouse.isHidden = false;
		}
		else {
			platform->hideCursor();
			mouse.isHidden = true;
		}
	}

	if (code == input::KeyCode::KEY_E && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
		camera.rotateRoll(-1);
	}

	if (code == input::KeyCode::KEY_Q && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
		camera.rotateRoll(1);
	}
}
