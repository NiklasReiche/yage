#pragma once

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <gml/gml.h>
#include <gl3d/camera.h>

#include <utility>

struct Mouse
{
	gml::Vec2f pos;
	float sensitivity = 0.2f;
	bool isHidden = false;
	bool first = true;
};

class MovementListener : public input::InputListener
{
public:
	MovementListener() = default;

	MovementListener(std::shared_ptr<platform::IWindow> window,
				  std::shared_ptr<gl3d::Camera> camera)
		: window(std::move(window)), camera(std::move(camera))
	{
		keyStates[input::KeyEvent::Code::KEY_W] = false;
		keyStates[input::KeyEvent::Code::KEY_A] = false;
		keyStates[input::KeyEvent::Code::KEY_S] = false;
		keyStates[input::KeyEvent::Code::KEY_D] = false;
		keyStates[input::KeyEvent::Code::KEY_LEFT] = false;
		keyStates[input::KeyEvent::Code::KEY_RIGHT] = false;
		keyStates[input::KeyEvent::Code::KEY_UP] = false;
		keyStates[input::KeyEvent::Code::KEY_DOWN] = false;
	}

	void onMousePosEvent(const input::MousePosEvent& event) override
	{
		float x = static_cast<const input::MousePosEvent&>(event).getXPos();
		float y = static_cast<const input::MousePosEvent&>(event).getYPos();

		if (mouse.isHidden) {
			gml::Vec2f dist = mouse.pos - gml::Vec2f(x, y);
			gml::Vec2f angle = dist * mouse.sensitivity;

			//gml::Quaternion<float> q_yaw = gml::quaternion::eulerAngle<float>(0, angle.x, 0);
			//gml::Quaternion<float> q_pitch = gml::quaternion::eulerAngle<float>(angle.y, 0, 0);

			//gml::Quaternion<float> newRotation = gml::normalize<float>(q_yaw * camera.rotation * q_pitch);
			camera->rotateYaw(angle.x());
			camera->rotatePitch(angle.y());
		}
		mouse.pos = gml::Vec2f(x, y);
	}

	void onKeyEvent(const input::KeyEvent& event) override
	{
		auto code = static_cast<const input::KeyEvent&>(event).getKey();
		auto action = static_cast<const input::KeyEvent&>(event).getAction();

		keyStates[code] = action != input::KeyEvent::Action::RELEASE;

		if (code == input::KeyEvent::Code::KEY_X && action == input::KeyEvent::Action::PRESS) {
			if (mouse.isHidden) {
				std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->showCursor();
				mouse.isHidden = false;
			} else {
				std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->hideCursor();
				mouse.isHidden = true;
			}
		}
	}

	void applyUpdate()
	{
		if (keyStates[input::KeyEvent::Code::KEY_W]) {
			camera->moveForward(speed);
		}
		if (keyStates[input::KeyEvent::Code::KEY_A]) {
			camera->moveLeft(speed);
		}
		if (keyStates[input::KeyEvent::Code::KEY_S]) {
			camera->moveBackward(speed);
		}
		if (keyStates[input::KeyEvent::Code::KEY_D]) {
			camera->moveRight(speed);
		}
	}

private:
	Mouse mouse;
	std::map<input::KeyEvent::Code, bool> keyStates;
	std::shared_ptr<platform::IWindow> window;
	std::shared_ptr<gl3d::Camera> camera;
    float speed = 0.01f;
};
