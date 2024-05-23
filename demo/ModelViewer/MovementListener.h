#pragma once

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <gml/gml.h>
#include <gl3d/camera.h>

#include <utility>
#include "ProjectionView.h"

struct Mouse
{
	gml::Vec2f pos;
	float sensitivity = 0.02f;
	bool first = true;
    bool pressed = false;
};

class MovementListener : public input::InputListener
{
public:
    std::shared_ptr<gl3d::SceneNode> world;
    std::shared_ptr<gl::IShader> pbrShader;
    std::shared_ptr<gl::IShader> pbrShader2;
    ProjectionView* m_projection_view;
    gml::Vec3f camPos;

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

		if (mouse.pressed) {
			gml::Vec2f dist = mouse.pos - gml::Vec2f(x, y);
			gml::Vec2f angle = dist * mouse.sensitivity;

            auto newPos =
                    gml::matrix::axisAngle(gml::Vec3f(0, 1, 0), angle.x()) *
                    gml::matrix::axisAngle(gml::Vec3f(1, 0, 0), angle.y()) *
                    gml::Vec4f(camPos.x(), camPos.y(), camPos.z(), 1);
            camPos = {newPos.x() / newPos.w(), newPos.y() / newPos.w(), newPos.z() / newPos.w()};

            m_projection_view->view = gml::matrix::lookAt<double>(camPos, gml::Vec3d(0, 0, 0));
            m_projection_view->syncView();
            pbrShader->setUniform("camPos", camPos);
            pbrShader2->setUniform("camPos", camPos);
		}
		mouse.pos = gml::Vec2f(x, y);
	}

	void onKeyEvent(const input::KeyEvent& event) override
	{
		auto code = static_cast<const input::KeyEvent&>(event).getKey();
		auto action = static_cast<const input::KeyEvent&>(event).getAction();

		keyStates[code] = action != input::KeyEvent::Action::RELEASE;

        if (code == input::KeyEvent::Code::KEY_MOUSE_1 && action == input::KeyEvent::Action::PRESS) {
            mouse.pressed = true;
        }
        if (code == input::KeyEvent::Code::KEY_MOUSE_1 && action == input::KeyEvent::Action::RELEASE) {
            mouse.pressed = false;
        }
	}

    void onMouseWheelEvent(const input::MouseWheelEvent& event) override
    {
        auto offset = event.getYOffset();
        if (offset < 0) {
            offset = 1 / (-offset + 1);
        } else {
            offset += 1;
        }
        world->setTransform(world->getTransform() * gml::matrix::scale<double>(offset, offset, offset));
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
