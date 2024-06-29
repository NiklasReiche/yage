#pragma once

#include <memory>
#include <map>

#include <core/input/InputListener.h>
#include <math/math.h>
#include <gl3d/camera.h>

class ModelViewerApp;

struct Mouse
{
	yage::math::Vec2f pos;
	float sensitivity = 0.02f;
	bool first = true;
    bool pressed = false;
};

class MovementListener final : public yage::input::InputListener
{
public:
	MovementListener() = default;

	explicit MovementListener(ModelViewerApp* app, std::shared_ptr<yage::gl3d::Camera> camera);

	void onMousePosEvent(const yage::input::MousePosEvent& event) override;

	void onKeyEvent(const yage::input::KeyEvent& event) override;

	void onMouseWheelEvent(const yage::input::MouseWheelEvent& event) override;

	void applyUpdate();

private:
	Mouse mouse;
	std::map<yage::input::KeyEvent::Code, bool> keyStates;
    float movement_speed = 0.01f;

	std::shared_ptr<yage::gl3d::Camera> camera;
	ModelViewerApp* app{};
};
