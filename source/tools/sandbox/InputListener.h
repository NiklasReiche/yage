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
	float sensitivity = 0.5f;
	bool isHidden = false;
	bool first = true;
};

class InputListener : public input::InputListener
{
public:
	InputListener() = default;

	InputListener(std::shared_ptr <platform::IWindow> window, std::shared_ptr <gl3d::Camera> camera)
		: window(std::move(window)), camera(std::move(camera))
	{
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

		if (code == input::KeyEvent::Code::KEY_W &&
		    (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS)) {
			camera->moveForward(0.1);
		}
		if (code == input::KeyEvent::Code::KEY_A &&
		    (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS)) {
			camera->moveLeft(0.1);
		}
		if (code == input::KeyEvent::Code::KEY_S &&
		    (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS)) {
			camera->moveBackward(0.1);
		}
		if (code == input::KeyEvent::Code::KEY_D &&
		    (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS)) {
			camera->moveRight(0.1);
		}
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

private:
	Mouse mouse;
	std::shared_ptr <platform::IWindow> window;
	std::shared_ptr <gl3d::Camera> camera;
};
