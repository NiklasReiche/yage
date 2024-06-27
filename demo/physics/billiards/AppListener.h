#pragma once

#include <memory>
#include <map>

#include <core/input/InputListener.h>
#include <math/math.h>
#include <gl3d/camera.h>


struct Mouse
{
    yage::math::Vec2f position;
    float sensitivity = 0.5f;
    bool is_captured = false;
};

class BilliardsApp;

class MovementListener final : public yage::input::InputListener
{
public:
    MovementListener() = default;

    MovementListener(std::shared_ptr<yage::gl3d::Camera> camera, BilliardsApp* app);

    void onMousePosEvent(const yage::input::MousePosEvent& event) override;

    void onKeyEvent(const yage::input::KeyEvent& event) override;

    void update();

private:
    Mouse mouse;
    float movement_speed = 0.05;
    std::map<yage::input::KeyEvent::Code, bool> keyStates;

    std::shared_ptr<yage::gl3d::Camera> camera;
    BilliardsApp* app{};
};
