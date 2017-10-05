#include "app.h"

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

	camera = graphics3d::Camera(gml::Vec3f(0.0f, 0.0f, 5.0f), gml::axisAngle<float>(0.0f, gml::Vec3f(0.0f, 0.0f, -1.0f)));
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
	skybox = graphics3d::SkyboxLoader(platform, glContext).loadSkybox(filenames, 100);
	skybox.shader.setUniform("skybox", 0);
	gml::Matrix4D<float> proj = gml::perspective<float>(45.0f, (float)(skyboxViewport.x + skyboxViewport.width)/ (float)(skyboxViewport.y + skyboxViewport.height), 0.1f, 1000.0f);
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

void App::updateCamera()
{
	
}

void App::on_mouse_pos_event(float x, float y)
{
	if (isLoaded && mouse.isHidden) {
		gml::Vec2f dist = mouse.pos - gml::Vec2f(x, y);
		gml::Vec2f angle = dist * mouse.sensitivity;

		gml::Quaternion<float> q_yaw = gml::axisAngle<float>(angle.x, gml::Vec3f(0.0f, 1.0f, 0.0f));
		gml::Quaternion<float> q_pitch = gml::axisAngle<float>(angle.y, gml::Vec3f(1.0f, 0.0f, 0.0f));

		gml::Quaternion<float> newRotation = gml::normalize<float>(q_yaw * camera.rotation * q_pitch);


		if (getPitch(newRotation) > 50) {
			camera.rotation = gml::normalize<float>(q_yaw * camera.rotation);
		}
		else {
			camera.rotation = newRotation;
		}
	}
	mouse.pos = gml::Vec2f(x, y);
}

float App::getPitch(gml::Quaternion<float> quaternion)
{
	gml::Vec3f worldUp(0, 1, 0);
	gml::Vec3f right = quaternion.getRight();
	gml::Vec3f worldForward = gml::cross(worldUp, right);
	gml::Vec3f forward = quaternion.getForward();
	return acos(gml::dot(worldForward, forward) / (gml::length(worldForward) * gml::length(forward))) * (180.0 / PI);
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
}