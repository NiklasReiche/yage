#pragma once

#include "platform/platform.h"
#include "graphics/gl.h"
#include "input/InputController.h"
#include "math/gml.h"

#include "graphics3d/camera.h"
#include "graphics3d/sceneRenderer.h"
#include "graphics3d/resourceManager.h"

struct Mouse
{
	gml::Vec2f pos;
	float sensitivity = 0.5f;
	bool isHidden = false;
	bool first = true;
};

class App
{
private:
	sys::PlatformHandle* platform;
	gl::GraphicsContext* glContext;
	input::InputController* inputController;
	input::InputListener* inputListener;
	Mouse mouse;

	gl::Shader shader;
	std::shared_ptr<gl3d::Camera> camera;
	gl3d::ResourceManager resourceManager;
	gl3d::SceneRenderer renderer;

	std::shared_ptr<gl3d::SceneGroup> scene;
	std::shared_ptr<gl3d::SceneGroup> cubeGroup;
	std::shared_ptr<gl3d::SceneObject> light1;
	std::shared_ptr<gl3d::SceneObject> cube1;

	int test = 0;

public:
	App()
	{
		platform = new sys::PlatformHandle();
		glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Sandbox");
		inputController = new input::InputController(platform);
		inputListener = inputController->addListener();
		inputListener->setOnMousePosEvent(std::bind(&App::on_mouse_pos_event, this, std::placeholders::_1, std::placeholders::_2));
		inputListener->setOnKeyEvent(std::bind(&App::on_key_event, this, std::placeholders::_1, std::placeholders::_2));

		renderer.gl = glContext;

		camera = std::make_shared<gl3d::Camera>(gl3d::Camera(gml::Vec3f(0.0f, 0.0f, 5.0f), gml::Quatd::eulerAngle(gml::PI, 0, 0)));

		glContext->setActiveViewport(gl::Viewport(0, 0, 1500, 900));


		std::string vertexShader =
#include "graphics3d/shaders/singleColorShader.vert"
			;
		std::string fragmentShader =
#include "graphics3d/shaders/singleColorShader.frag"
			;
		gl::ShaderLoader shaderLoader(platform, glContext);
		shader = shaderLoader.loadFromString(vertexShader, fragmentShader);

		gml::Mat4f proj = gml::Mat4f::perspective(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
		shader.setUniform("projection", proj);

		setupGraph();
		glContext->enableDepthTest();
	}

	void setupGraph()
	{
		std::vector<float> vertices = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		gl::Drawable cube = glContext->createDrawable(vertices, { 3, 3 }, gl::DrawMode::DRAW_STATIC);
		std::shared_ptr<gl3d::Mesh> cubeMesh = std::make_shared<gl3d::Mesh>();;
		cubeMesh->drawable = cube;

		std::shared_ptr<gl3d::Material> ruby = std::make_shared<gl3d::Material>();
		ruby->addVec3("ambient", gml::Vec3f(0.1));
		ruby->addVec3("diffuse", gml::Vec3f(0.61f, 0.04f, 0.04f));
		ruby->addVec3("specular", gml::Vec3f(0.7f, 0.6f, 0.6));
		ruby->addFloat("shininess", 32.0f);
		ruby->setShader(shader);


		std::shared_ptr<gl3d::PointLight> lightRes = 
			std::make_shared<gl3d::PointLight>(gl3d::PointLight(
				{ gml::Vec3f(0.2f), gml::Vec3f(0.5f), gml::Vec3f(1.0f) },
				{ 1.0f, 0.09f, 0.032f }));

		cube1 = std::make_shared<gl3d::SceneObject>("cube");
		cube1->bindMaterial(ruby);
		cube1->bindMesh(cubeMesh);

		light1 = std::make_shared<gl3d::SceneObject>("light");
		light1->setTransform(gml::Mat4d::quaternion(gml::Quatd::eulerAngle(0, 0, 0)) * gml::Mat4d::translate(gml::Vec3d(0.0, 0.0, -3.0)) * gml::Mat4d::scale(0.1f));
		light1->bindMaterial(ruby);
		light1->bindMesh(cubeMesh);
		light1->bindLight(lightRes);
		light1->bindCamera(camera);

		scene = std::make_shared<gl3d::SceneGroup>("world");
		cubeGroup = std::make_shared<gl3d::SceneGroup>("cubeGroup");

		scene->addChild(cubeGroup);
		scene->addChild(cube1);
		cubeGroup->addChild(light1);
	}

	void run()
	{
		double coord = -1.0;
		glContext->showWindow();
		platform->getTimeStep();
		while (!platform->shouldDestroy())
		{
			double dt = platform->getTimeStep();
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			coord += 0.5 * dt;

			gml::Mat4d localTransform =
				gml::Mat4d::axisAngle(gml::Vec3d(1.0, 0.0, 0.0), coord) *
				gml::Mat4d::translate(gml::Vec3d(0.0, 0.0, -3.0)) * 
				gml::Mat4d::scale(0.1f);

			light1->setTransform(localTransform);

			glContext->useShader(shader);
			shader.setUniform("view", camera->getViewMatrix());
			shader.setUniform("viewPos", camera->getPosition());

			renderer.renderGraph(scene);

			glContext->swapBuffers();
			inputController->poll();
		}
	}

	void on_mouse_pos_event(float x, float y)
	{
		if ( mouse.isHidden) {
			gml::Vec2f dist = mouse.pos - gml::Vec2f(x, y);
			gml::Vec2f angle = dist * mouse.sensitivity;

			gml::Quaternion<float> q_yaw = gml::Quatf::eulerAngle(0, angle.x, 0);
			gml::Quaternion<float> q_pitch = gml::Quatf::eulerAngle(angle.y, 0, 0);

			//gml::Quaternion<float> newRotation = gml::normalize<float>(q_yaw * camera.rotation * q_pitch);
			camera->rotateYaw(angle.x);
			camera->rotatePitch(angle.y);
		}
		mouse.pos = gml::Vec2f(x, y);
	}
	void on_key_event(input::KeyCode code, input::KeyAction action)
	{
		if (code == input::KeyCode::KEY_W && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera->moveForward(0.1);
		}
		if (code == input::KeyCode::KEY_A && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera->moveLeft(0.1);
		}
		if (code == input::KeyCode::KEY_S && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera->moveBackward(0.1);
		}
		if (code == input::KeyCode::KEY_D && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera->moveRight(0.1);
		}
		if (code == input::KeyCode::KEY_1 && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			test += 10;
		}
		if (code == input::KeyCode::KEY_X && action == input::KeyAction::PRESS) {
			if (mouse.isHidden) {
				platform->showCursor();
				mouse.isHidden = false;
			}
			else {
				platform->hideCursor();
				mouse.isHidden = true;
			}
		}
	}
};
