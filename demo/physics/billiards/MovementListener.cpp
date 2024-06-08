#include "MovementListener.h"

#include "app.h"

MovementListener::MovementListener(std::shared_ptr<platform::IWindow> window, std::shared_ptr<gl3d::Camera> camera,
                                   physics3d::Simulation* sim, App* app)
        : window(std::move(window)), camera(std::move(camera)), sim(sim), app(app)
{
    keyStates[input::KeyEvent::Code::KEY_W] = false;
    keyStates[input::KeyEvent::Code::KEY_A] = false;
    keyStates[input::KeyEvent::Code::KEY_S] = false;
    keyStates[input::KeyEvent::Code::KEY_D] = false;
}

void MovementListener::onMousePosEvent(const input::MousePosEvent& event)
{
    float x = static_cast<const input::MousePosEvent&>(event).getXPos();
    float y = static_cast<const input::MousePosEvent&>(event).getYPos();

    if (mouse.isHidden) {
        gml::Vec2d dist = mouse.pos - gml::Vec2f(x, y);
        gml::Vec2d angle = dist * mouse.sensitivity;

        gml::Quatd q_yaw = gml::quaternion::eulerAngle<double>(gml::to_rad(angle.x()), 0.0, 0.0);
        gml::Quatd q_pitch = gml::quaternion::eulerAngle<double>(0.0, 0.0, -gml::to_rad(angle.y()));
        gml::Quatd rotation = gml::normalize(q_yaw * camera->getRotation() * q_pitch);
        camera->rotateTo(rotation);
    }
    mouse.pos = gml::Vec2f(x, y);
}

void MovementListener::applyUpdate()
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

void MovementListener::onKeyEvent(const input::KeyEvent& event)
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

    if (code == input::KeyEvent::Code::KEY_BACKSLASH && (action == input::KeyEvent::Action::PRESS ||
                                                         action == input::KeyEvent::Action::REPEAT)) {
        speed *= 0.9;
    }
    if (code == input::KeyEvent::Code::KEY_RIGHT_BRACKET && (action == input::KeyEvent::Action::PRESS ||
                                                             action == input::KeyEvent::Action::REPEAT)) {
        speed *= 1.1;
    }

    if (code == input::KeyEvent::Code::KEY_ENTER && action == input::KeyEvent::Action::PRESS && ball != nullptr) {
        ball->apply_force(gml::Vec3d(35, 0, 0), ball->position());
    }

    if (code == input::KeyEvent::Code::KEY_RIGHT && (action == input::KeyEvent::Action::PRESS ||
                                                     action == input::KeyEvent::Action::REPEAT)) {
        if (app->visualize) {
            sim->update_staggered(1. / 60);
        } else{
            sim->update(1. / 60.);
        }
    }

    if (code == input::KeyEvent::Code::KEY_SPACE && action == input::KeyEvent::Action::PRESS) {
        app->simulate = !app->simulate;
    }

    if (code == input::KeyEvent::Code::KEY_V && action == input::KeyEvent::Action::PRESS) {
        app->visualize = !app->visualize;
    }
}
