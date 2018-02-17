#include <platform/platform.h>
#include <graphics/gl.h>
#include "input/InputController.h"
#include "math/MVector.h"
#include "graphics3d/skybox.h"
#include "graphics3d/camera.h"

#include "gui.h"

enum Side
{
	RIGHT,
	LEFT,
	TOP,
	BOTTOM,
	BACK,
	FRONT
};

struct Mouse
{
	gml::Vec2f pos;
	float sensitivity = 0.5f;
	bool isHidden = false;
};

class App
{
private:
	sys::PlatformHandle* platform;
	gl::GraphicsContext* glContext;
	input::InputController* inputController;
	input::InputListener* inputListener;
	Gui* gui;

	gl::Viewport guiViewport;
	gl::Viewport skyboxViewport;

	gl3d::Skybox skybox;
	bool isLoaded = false;

	gl3d::Camera camera;
	Mouse mouse;

	void on_mouse_pos_event(float x, float y);
	void on_key_event(input::KeyCode code, input::KeyAction action);

	float getPitch(gml::Quaternion<float> quaternion);

public:
	App();
	~App();
	
	int run();

	void updateCamera();

	void loadSkybox(std::array<std::string, 6> filenames);
	void drawSkybox();
};
