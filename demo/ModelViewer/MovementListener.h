#pragma once

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <math/math.h>
#include <gl3d/camera.h>

#include <utility>

using namespace yage;

struct Mouse
{
	math::Vec2f pos;
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
			math::Vec2f dist = mouse.pos - math::Vec2f(x, y);
			math::Vec2f angle = dist * mouse.sensitivity;

			math::Vec3f camPos = static_cast<math::Vec3f>(camera->position());
            auto x_axis = std::abs(camPos.z()) > std::abs(camPos.x()) ? math::Vec3f(1, 0, 0) : math::Vec3f(0, 0, 1);

            auto newPos =
                    math::matrix::axisAngle(math::Vec3f(0, 1, 0), angle.x()) *
                    math::matrix::axisAngle(x_axis, angle.y()) *
                    math::Vec4f(camPos.x(), camPos.y(), camPos.z(), 1);
            camPos = {newPos.x() / newPos.w(), newPos.y() / newPos.w(), newPos.z() / newPos.w()};

            camera->look_at(static_cast<math::Vec3d>(camPos), math::Vec3d(0.0));
		}
		mouse.pos = math::Vec2f(x, y);
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
        world->local_transform = world->local_transform * math::matrix::scale<double>(offset, offset, offset);
    }

	void applyUpdate()
	{
		if (keyStates[input::KeyEvent::Code::KEY_W]) {
			camera->move_forward(speed);
		}
		if (keyStates[input::KeyEvent::Code::KEY_A]) {
			camera->move_left(speed);
		}
		if (keyStates[input::KeyEvent::Code::KEY_S]) {
			camera->move_backward(speed);
		}
		if (keyStates[input::KeyEvent::Code::KEY_D]) {
			camera->move_right(speed);
		}
	}

private:
	Mouse mouse;
	std::map<input::KeyEvent::Code, bool> keyStates;
	std::shared_ptr<platform::IWindow> window;
	std::shared_ptr<gl3d::Camera> camera;
    float speed = 0.01f;
};
