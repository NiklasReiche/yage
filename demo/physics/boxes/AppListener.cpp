#include "AppListener.h"

#include "BoxApp.h"

using namespace yage;

AppListener::AppListener(std::shared_ptr<gl3d::Camera> camera, BoxApp* app)
        : camera(std::move(camera)), app(app)
{
    keyStates[input::KeyEvent::Code::KEY_W] = false;
    keyStates[input::KeyEvent::Code::KEY_A] = false;
    keyStates[input::KeyEvent::Code::KEY_S] = false;
    keyStates[input::KeyEvent::Code::KEY_D] = false;
}

void AppListener::onMousePosEvent(const input::MousePosEvent& event)
{
    const float x = event.getXPos();
    const float y = event.getYPos();

    if (mouse.is_captured) {
        const math::Vec2f dist = mouse.position - math::Vec2f(x, y);
        math::Vec2f angle = dist * mouse.sensitivity;

        const math::Quatd q_yaw = math::quaternion::euler_angle<double>(math::to_rad(angle.x()), 0.0, 0.0);
        const math::Quatd q_pitch = math::quaternion::euler_angle<double>(0.0, 0.0, -math::to_rad(angle.y()));
        const math::Quatd rotation = normalize(q_yaw * camera->rotation() * q_pitch);
        camera->rotate_to(rotation);
    }
    mouse.position = math::Vec2f(x, y);
}

void AppListener::update()
{
    if (keyStates[input::KeyEvent::Code::KEY_W]) {
        camera->move_forward(movement_speed);
    }
    if (keyStates[input::KeyEvent::Code::KEY_A]) {
        camera->move_left(movement_speed);
    }
    if (keyStates[input::KeyEvent::Code::KEY_S]) {
        camera->move_backward(movement_speed);
    }
    if (keyStates[input::KeyEvent::Code::KEY_D]) {
        camera->move_right(movement_speed);
    }

    if (keyStates[input::KeyEvent::Code::KEY_BACKSLASH]) {
        movement_speed *= 0.9;
    }
    if (keyStates[input::KeyEvent::Code::KEY_RIGHT_BRACKET]) {
        movement_speed *= 1.1;
    }
}

void AppListener::onKeyEvent(const input::KeyEvent& event)
{
    const auto code = event.getKey();
    const auto action = event.getAction();

    keyStates[code] = action != input::KeyEvent::Action::RELEASE;

    if (code == input::KeyEvent::Code::KEY_X && action == input::KeyEvent::Action::PRESS) {
        app->toggle_mouse_capture();
        if (mouse.is_captured) {
            mouse.is_captured = false;
        } else {
            mouse.is_captured = true;
        }
    }

    if (code == input::KeyEvent::Code::KEY_RIGHT && (action == input::KeyEvent::Action::PRESS ||
                                                     action == input::KeyEvent::Action::REPEAT)) {
        app->step_simulation();
    }

    if (code == input::KeyEvent::Code::KEY_SPACE && action == input::KeyEvent::Action::PRESS) {
        app->toggle_simulation();
    }

    if (code == input::KeyEvent::Code::KEY_V && action == input::KeyEvent::Action::PRESS) {
        app->toggle_visualization();
    }

    if (code == input::KeyEvent::Code::KEY_B && action == input::KeyEvent::Action::PRESS) {
        app->add_box();
    }

    if (code == input::KeyEvent::Code::KEY_N && action == input::KeyEvent::Action::PRESS) {
        app->add_box_rotated();
    }

    if (code == input::KeyEvent::Code::KEY_ENTER && action == input::KeyEvent::Action::PRESS) {
        app->throw_box();
    }

    if (code == input::KeyEvent::Code::KEY_R && action == input::KeyEvent::Action::PRESS) {
        app->reset();
    }
}
