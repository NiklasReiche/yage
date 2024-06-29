#include "MovementListener.h"

#include "ModelViewerApp.h"

using namespace yage;

MovementListener::MovementListener(ModelViewerApp* app, std::shared_ptr<gl3d::Camera> camera): camera(std::move(camera)), app(app)
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

void MovementListener::applyUpdate()
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
}

void MovementListener::onMousePosEvent(const input::MousePosEvent& event)
{
    const float x = event.getXPos();
    const float y = event.getYPos();

    if (mouse.pressed) {
        const math::Vec2f dist = mouse.pos - math::Vec2f(x, y);
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

void MovementListener::onKeyEvent(const input::KeyEvent& event)
{
    const auto code = event.getKey();
    const auto action = event.getAction();

    keyStates[code] = action != input::KeyEvent::Action::RELEASE;

    if (code == input::KeyEvent::Code::KEY_MOUSE_1 && action == input::KeyEvent::Action::PRESS) {
        mouse.pressed = true;
    }
    if (code == input::KeyEvent::Code::KEY_MOUSE_1 && action == input::KeyEvent::Action::RELEASE) {
        mouse.pressed = false;
    }
}

void MovementListener::onMouseWheelEvent(const input::MouseWheelEvent& event)
{
    float offset = event.getYOffset();
    if (offset < 0) {
        offset = 1 / (-offset + 1);
    } else {
        offset += 1;
    }
    app->scale_scene(offset);
}
