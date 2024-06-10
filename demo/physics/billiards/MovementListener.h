#pragma once

#include "core/platform/Window.h"
#include "core/platform/desktop/GlfwWindow.h"
#include "core/gl/Context.h"
#include "math/math.h"
#include "gl3d/camera.h"
#include "physics3d/RigidBody.h"
#include "physics3d/Simulation.h"

#include <map>
#include <utility>

using namespace yage;

struct Mouse
{
    math::Vec2f pos;
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
                     App* app);

    void onMousePosEvent(const input::MousePosEvent& event) override;

    void onKeyEvent(const input::KeyEvent& event) override;

    void applyUpdate();

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
