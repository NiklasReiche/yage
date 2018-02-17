#pragma once

#include "platform/platform.h"
#include "graphics/gl.h"
#include "input/InputController.h"
#include "math/MVector.h"

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
	gl3d::Camera camera;
	gl3d::ResourceManager resourceManager;
	gl3d::SceneRenderer renderer;

	int MATERIAL_CODE = 1;
	int DRAWABLE_CODE = 2;
	int LIGHT_CODE = 3;

	gl3d::SceneGroup scene;
	gl3d::SceneGroup group1;
	gl3d::SceneObject cube1;
	gl3d::SceneObject light1;
	gl3d::SceneGroup group2;
	gl3d::SceneObject cube2;
	gl3d::SceneObject cube3;
	gl3d::SceneGroup group3;

public:
	App()
	{
		platform = new sys::PlatformHandle();
		glContext = new gl::GraphicsContext(platform, 1500, 900, "YAGE Sandbox");
		inputController = new input::InputController(platform);
		inputListener = inputController->addListener();
		inputListener->setOnMousePosEvent(std::bind(&App::on_mouse_pos_event, this, std::placeholders::_1, std::placeholders::_2));
		inputListener->setOnKeyEvent(std::bind(&App::on_key_event, this, std::placeholders::_1, std::placeholders::_2));
		mouse.isHidden = true;

		renderer.gl = glContext;
		renderer.resourceManager = &resourceManager;

		camera = gl3d::Camera(gml::Vec3f(0.0f, 0.0f, 2.0f), gml::axisAngle<float>(0.0f, gml::Vec3f(0.0f, 0.0f, -1.0f)));

		glContext->setActiveViewport(gl::Viewport(0, 0, 1500, 900));


		std::string vertexShader =
#include "graphics3d/shaders/singleColorShader.vert"
			;
		std::string fragmentShader =
#include "graphics3d/shaders/singleColorShader.frag"
			;
		gl::ShaderLoader shaderLoader(platform, glContext);
		shader = shaderLoader.loadFromString(vertexShader, fragmentShader);
		renderer.shader = shader;

		gml::Matrix4D<float> proj = gml::perspective<float>(45.0f, (float)(1500) / (float)(900), 0.1f, 1000.0f);
		shader.setUniform("projection", proj);
		shader.setUniform("viewPos", camera.getPosition());

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

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

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
		gl3d::Mesh cubeMesh;
		cubeMesh.drawable = cube;
		unsigned int id_cube = resourceManager.add(cubeMesh);

		gl3d::Material ruby;
		ruby.addVec3("ambient", gml::Vec3f(0.1));
		ruby.addVec3("diffuse", gml::Vec3f(0.61f, 0.04f, 0.04f));
		ruby.addVec3("specular", gml::Vec3f(0.7f, 0.6f, 0.6));
		ruby.addFloat("shininess", 0.6f);
		ruby.setShader(shader);
		unsigned int id_ruby = resourceManager.add(ruby);

		gl3d::PointLight light;
		light.color.ambient = gml::Vec3f(0.2f);
		light.color.diffuse = gml::Vec3f(0.5f);
		light.color.specular = gml::Vec3f(1.0f);
		light.constants.constant = 1.0f;
		light.constants.linear = 0.09f;
		light.constants.quadratic = 0.032f;
		unsigned int id_light = resourceManager.add(light);

		cube1 = gl3d::SceneObject();
		cube1.addId(gl3d::SceneObjectIdCode::MATERIAL, id_ruby);
		cube1.addId(gl3d::SceneObjectIdCode::MESH, id_cube);

		cube2 = gl3d::SceneObject();
		cube2.addId(gl3d::SceneObjectIdCode::MATERIAL, id_ruby);
		cube2.addId(gl3d::SceneObjectIdCode::MESH, id_cube);

		light1 = gl3d::SceneObject();
		light1.addId(gl3d::SceneObjectIdCode::LIGHT, id_light);

		cube3 = gl3d::SceneObject();
		cube3.addId(gl3d::SceneObjectIdCode::MATERIAL, id_ruby);
		cube3.addId(gl3d::SceneObjectIdCode::MESH, id_cube);

		group1.setTransform(gml::translate<float>(gml::Vec3f(0.0f, 1.5f, 0.0f)));
		group1.addChild(&cube1);

		group3.addChild(&light1);
		group3.addChild(&cube3);

		group2.setTransform(gml::translate<float>(gml::Vec3f(0.0f, 0.0f, -3.0f)));
		group2.addChild(&cube2);

		group2.addChild(&group1);
		group2.addChild(&group3);

		scene.addChild(&group2);
	}

	void run()
	{
		float coord = -1.0f;
		glContext->showWindow();
		platform->getTimeStep();
		while (!platform->shouldDestroy())
		{
			double dt = platform->getTimeStep();
			glClearColor(0, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glContext->useShader(shader);
			shader.setUniform("view", camera.getViewMatrix());
			shader.setUniform("viewPos", camera.getPosition());

			coord += 0.1 * dt;
			gml::Matrix4D<float> globalTransform = gml::translate<float>(gml::Vec3f(0.0f, 0.0f, -3.0f)) * gml::rotate<float>(coord * 100, gml::Vec3f(0.0f, 0.0f, -1.0f));
			gml::Matrix4D<float> localTransform = gml::translate<float>(gml::Vec3f(0.0f, 1.5f, 0.0f)) * gml::rotate<float>(coord * 100, gml::Vec3f(0.0f, 0.0f, -1.0f));

			group2.setTransform(globalTransform);
			group1.setTransform(localTransform);
			localTransform = /*gml::rotate<float>(coord * 300, gml::Vec3f(1.0f, 0.0f, 1.0f)) * */ gml::translate<float>(gml::Vec3f(0.0f, 0.0f, 2.0f)) * gml::scale<float>(0.1f);
			group3.setTransform(localTransform);

			renderer.renderGraph(&scene);

			glContext->swapBuffers();
			inputController->poll();
		}
	}

	void on_mouse_pos_event(float x, float y)
	{
		if (!mouse.first) {
			gml::Vec2f dist = mouse.pos - gml::Vec2f(x, y);
			gml::Vec2f angle = dist * mouse.sensitivity;

			gml::Quaternion<float> q_yaw = gml::eulerAngle<float>(0, angle.x, 0);
			gml::Quaternion<float> q_pitch = gml::eulerAngle<float>(angle.y, 0, 0);

			//gml::Quaternion<float> newRotation = gml::normalize<float>(q_yaw * camera.rotation * q_pitch);
			camera.rotateYaw(angle.x);
			camera.rotatePitch(angle.y);
			mouse.isHidden = false;
		}
		else {
			mouse.first = false;
		}
		mouse.pos = gml::Vec2f(x, y);
	}
	void on_key_event(input::KeyCode code, input::KeyAction action)
	{
		if (code == input::KeyCode::KEY_W && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera.moveForward(0.1);
		}
		if (code == input::KeyCode::KEY_A && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera.moveLeft(0.1);
		}
		if (code == input::KeyCode::KEY_S && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera.moveBackward(0.1);
		}
		if (code == input::KeyCode::KEY_D && (action == input::KeyAction::REPEAT || action == input::KeyAction::PRESS)) {
			camera.moveRight(0.1);
		}
	}
};
