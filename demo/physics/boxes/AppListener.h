#pragma once

#include <map>
#include <memory>

#include <core/input/InputListener.h>
#include <gl3d/camera.h>

class NewApp;

struct Mouse
{
    yage::math::Vec2f position;
    float sensitivity = 0.5f;
    bool is_captured = false;
};

class AppListener final : public yage::input::InputListener
{
public:
    AppListener() = default;

    AppListener(std::shared_ptr<yage::gl3d::Camera> camera, NewApp* app);

    void onMousePosEvent(const yage::input::MousePosEvent& event) override;

    void onKeyEvent(const yage::input::KeyEvent& event) override;

    void update();

private:
    Mouse mouse;
    float movement_speed = 1;
    std::map<yage::input::KeyEvent::Code, bool> keyStates;

    std::shared_ptr<yage::gl3d::Camera> camera;
    NewApp* app{};
};
