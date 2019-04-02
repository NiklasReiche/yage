#pragma once

#include "platform/platform.h"
#include "graphics/gl.h"
#include "input/InputController.h"
#include "math/gml.h"
#include "graphics2d/sprite.h"
#include "image/image.h"
#include "image/imageReader.h"
#include "graphics2d/renderer.h"

struct Mouse
{
	gml::Vec2f pos;
	float sensitivity = 0.5f;
	bool isHidden = false;
	bool first = true;
};

class App
{
private:
	sys::PlatformHandle* platform;
	gl::GraphicsContext* glContext;
	input::InputController* inputController;
	input::InputListener* inputListener;
	gl2d::Renderer* renderer;
	Mouse mouse;

	gl::Shader shader;

	int test = 0;

public:
	App()
	{
		platform = new sys::PlatformHandle();
		glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Sandbox2D");
		inputController = new input::InputController(platform);
		inputListener = inputController->addListener();
		inputListener->setOnMousePosEvent(std::bind(&App::on_mouse_pos_event, this, std::placeholders::_1, std::placeholders::_2));
		inputListener->setOnKeyEvent(std::bind(&App::on_key_event, this, std::placeholders::_1, std::placeholders::_2));
		renderer = new gl2d::Renderer(glContext);

		glContext->setActiveViewport(gl::Viewport(0, 0, 1500, 900));


		std::string vertexShader =
#include "graphics2d/shaders/shader2d_GL3.vert"
			;
		std::string fragmentShader =
#include "graphics2d/shaders/shader2d_GL3.frag"
			;
		gl::ShaderLoader shaderLoader(platform, glContext);
		shader = shaderLoader.loadFromString(vertexShader, fragmentShader);
		shader.setUniform("projection", gml::Mat4f::orthographic(0, 1500, 0, 900, 0, 100));

		glContext->enableDepthTest();
	}

	void run()
	{
		img::Image spriteImage = img::ImageReader(platform).readFile("D:\\DEV\\Projects\\YAGE\\source\\tools\\sandbox2d\\assets\\spaceship.png");
		gl::Texture spriteTexture = glContext->create2DTexture(spriteImage.getRawData(), spriteImage.getWidth(), spriteImage.getHeight(), gl::ImageFormat::RGB);
		gl2d::Sprite sprite(gml::Vec2f(500, 500), gml::Vec2f(100, 100), spriteTexture, glContext);

		glContext->showWindow();
		platform->getTimeStep();
		while (!platform->shouldDestroy())
		{
			double dt = platform->getTimeStep();
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glContext->useShader(shader);
			renderer->drawSprite(sprite);

			glContext->swapBuffers();
			inputController->poll();
		}
	}

	void on_mouse_pos_event(float x, float y)
	{
		if ( mouse.isHidden) {
			gml::Vec2f dist = mouse.pos - gml::Vec2f(x, y);
			gml::Vec2f angle = dist * mouse.sensitivity;

			gml::Quaternion<float> q_yaw = gml::Quatf::eulerAngle(0, angle.x, 0);
			gml::Quaternion<float> q_pitch = gml::Quatf::eulerAngle(angle.y, 0, 0);

			//gml::Quaternion<float> newRotation = gml::normalize<float>(q_yaw * camera.rotation * q_pitch);
		}
		mouse.pos = gml::Vec2f(x, y);
	}
	void on_key_event(input::KeyCode code, input::KeyAction action)
	{
		if (code == input::KeyCode::KEY_W && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			
		}
		if (code == input::KeyCode::KEY_A && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			
		}
		if (code == input::KeyCode::KEY_S && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			
		}
		if (code == input::KeyCode::KEY_D && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			
		}
		if (code == input::KeyCode::KEY_1 && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			test += 10;
		}
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
};
