#pragma once

#include <platform/Window.h>
#include <platform/desktop/GlfwWindow.h>
#include <math/gml.h>

#include <graphics3d/camera.h>
#include <graphics3d/sceneRenderer.h>
#include <graphics3d/resourceManager.h>

struct Mouse
{
	gml::Vec2f pos;
	float sensitivity = 0.5f;
	bool isHidden = false;
	bool first = true;
};

class InputListener : public input::InputListener
{
public:
	InputListener() = default;
	InputListener(sys::Window window, std::shared_ptr<gl3d::Camera> camera)
		: window(window), camera(camera)
	{
		
	}

	void onMousePosEvent(const input::MousePosEvent& event) override
	{
		float x = static_cast<const input::MousePosEvent&>(event).getXPos();
		float y = static_cast<const input::MousePosEvent&>(event).getYPos();

		if (mouse.isHidden)
		{
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
	void onKeyEvent(const input::KeyEvent& event) override
	{
		auto code = static_cast<const input::KeyEvent&>(event).getKey();
		auto action = static_cast<const input::KeyEvent&>(event).getAction();

		if (code == input::KeyEvent::Code::KEY_W && (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS))
		{
			camera->moveForward(0.1);
		}
		if (code == input::KeyEvent::Code::KEY_A && (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS))
		{
			camera->moveLeft(0.1);
		}
		if (code == input::KeyEvent::Code::KEY_S && (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS))
		{
			camera->moveBackward(0.1);
		}
		if (code == input::KeyEvent::Code::KEY_D && (action == input::KeyEvent::Action::REPEAT || action == input::KeyEvent::Action::PRESS))
		{
			camera->moveRight(0.1);
		}
		if (code == input::KeyEvent::Code::KEY_X && action == input::KeyEvent::Action::PRESS)
		{
			if (mouse.isHidden)
			{
				std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->showCursor();
				mouse.isHidden = false;
			}
			else
			{
				std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->hideCursor();
				mouse.isHidden = true;
			}
		}
	}

private:
	Mouse mouse;
	sys::Window window;
	std::shared_ptr<gl3d::Camera> camera;
};

class App
{
private:
	sys::Window window;
	gl::Context glContext;
	InputListener inputListener;
	

	gl::Shader shader;
	std::shared_ptr<gl3d::Camera> camera;
	gl3d::ResourceManager resourceManager;
	gl3d::SceneRenderer renderer;

	std::shared_ptr<gl3d::SceneGroup> scene;
	std::shared_ptr<gl3d::SceneGroup> cubeGroup;
	std::shared_ptr<gl3d::SceneObject> light1;
	std::shared_ptr<gl3d::SceneObject> cube1;

public:
	App()
	{
		window = std::make_shared<sys::desktop::GlfwWindow>();
		glContext = std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->createContext(1500, 900, "YAGE Sandbox");

		camera = std::make_shared<gl3d::Camera>(gl3d::Camera(gml::Vec3f(0.0f, 0.0f, 5.0f), gml::Quatd::eulerAngle(gml::PI, 0, 0)));

		inputListener = InputListener(window, camera);
		window->attach(inputListener);

		renderer.renderer = glContext->getRenderer();
		renderer.renderer->setClearColor(0x008080FFu);
		renderer.renderer->enableDepthTest();
		renderer.renderer->setViewport(0, 0, 1500, 900);


		std::string vertexShader =
#include "graphics3d/shaders/singleColorShader.vert"
			;
		std::string fragmentShader =
#include "graphics3d/shaders/singleColorShader.frag"
			;

		shader = glContext->getShaderCreator()->createShader(vertexShader, fragmentShader);

		gml::Mat4f proj = gml::Mat4f::perspective(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
		shader->setUniform("projection", proj);

		setupGraph();
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

		gl::Drawable cube = glContext->getDrawableCreator()->createDrawable(vertices, { 3, 3 }, gl::VertexFormat::INTERLEAVED);
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
		std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->showWindow();
		std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->getTimeStep();
		while (!window->shouldDestroy())
		{
			double dt = std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->getTimeStep();
			
			renderer.renderer->clear();
			
			coord += 0.5 * dt;

			gml::Mat4d localTransform =
				gml::Mat4d::axisAngle(gml::Vec3d(1.0, 0.0, 0.0), coord) *
				gml::Mat4d::translate(gml::Vec3d(0.0, 0.0, -3.0)) * 
				gml::Mat4d::scale(0.1f);

			light1->setTransform(localTransform);

			renderer.renderer->useShader(shader);
			shader->setUniform("view", camera->getViewMatrix());
			shader->setUniform("viewPos", camera->getPosition());

			renderer.renderGraph(scene);

			std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->swapBuffers();
			std::static_pointer_cast<sys::desktop::GlfwWindow>(window)->pollEvents();
		}
	}
};
