#pragma once

#include "core/platform/Window.h"
#include "core/platform/desktop/GlfwWindow.h"
#include "core/gl/Context.h"
#include "gml/gml.h"
#include "gl3d/camera.h"
#include "physics3d/RigidBody.h"
#include "physics3d/Simulation.h"

#include <map>
#include <utility>

struct Mouse
{
    gml::Vec2f pos;
    float sensitivity = 0.5f;
    bool isHidden = false;
    bool first = true;
};

class App;

class MovementListener : public input::InputListener
{
public:
    MovementListener() = default;

    MovementListener(std::shared_ptr<platform::IWindow> window,
                     std::shared_ptr<gl3d::Camera> camera,
                     physics3d::Simulation* sim,
                     App* app)
            : window(std::move(window)), camera(std::move(camera)), sim(sim), app(app)
    {
        keyStates[input::KeyEvent::Code::KEY_W] = false;
        keyStates[input::KeyEvent::Code::KEY_A] = false;
        keyStates[input::KeyEvent::Code::KEY_S] = false;
        keyStates[input::KeyEvent::Code::KEY_D] = false;
    }

    void onMousePosEvent(const input::MousePosEvent& event) override
    {
        float x = static_cast<const input::MousePosEvent&>(event).getXPos();
        float y = static_cast<const input::MousePosEvent&>(event).getYPos();

        if (mouse.isHidden) {
            gml::Vec2d dist = mouse.pos - gml::Vec2f(x, y);
            gml::Vec2d angle = dist * mouse.sensitivity;

            gml::Quatd q_yaw = gml::quaternion::eulerAngle<double>(gml::toRad(angle.x()), 0.0, 0.0);
            gml::Quatd q_pitch = gml::quaternion::eulerAngle<double>(0.0, 0.0, -gml::toRad(angle.y()));
            gml::Quatd rotation = gml::normalize(q_yaw * camera->getRotation() * q_pitch);
            camera->rotateTo(rotation);
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

        if (code == input::KeyEvent::Code::KEY_BACKSLASH && (action == input::KeyEvent::Action::PRESS ||
                                                             action == input::KeyEvent::Action::REPEAT)) {
            speed *= 0.9;
        }
        if (code == input::KeyEvent::Code::KEY_RIGHT_BRACKET && (action == input::KeyEvent::Action::PRESS ||
                                                                 action == input::KeyEvent::Action::REPEAT)) {
            speed *= 1.1;
        }

        if (code == input::KeyEvent::Code::KEY_ENTER && action == input::KeyEvent::Action::PRESS && ball != nullptr) {
            ball->applyForce(gml::Vec3d(10, 0, 0), ball->getPosition());
        }

        if (code == input::KeyEvent::Code::KEY_G && action == input::KeyEvent::Action::PRESS) {
            sim->enable_gravity();
        }
        if (code == input::KeyEvent::Code::KEY_RIGHT && (action == input::KeyEvent::Action::PRESS ||
                                                         action == input::KeyEvent::Action::REPEAT)) {
            sim->integrate(1. / 60);
        }
        if (code == input::KeyEvent::Code::KEY_SPACE && action == input::KeyEvent::Action::PRESS) {
            toggle_simulation();
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

    void toggle_simulation();

public:
    physics3d::RigidBody* ball{};

private:
    Mouse mouse;
    float speed = 0.05;
    std::map<input::KeyEvent::Code, bool> keyStates;
    std::shared_ptr<platform::IWindow> window;
    std::shared_ptr<gl3d::Camera> camera;
    physics3d::Simulation* sim{};
    App* app{};
};
