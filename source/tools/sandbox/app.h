#pragma once

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <gml/gml.h>

#include <gl3d/camera.h>
#include <gl3d/sceneRenderer.h>
#include <gl3d/resourceManager.h>

#include <physics3d/Simulation.h>

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
	InputListener(std::shared_ptr<platform::IWindow> window, std::shared_ptr<gl3d::Camera> camera)
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

			//gml::Quaternion<float> q_yaw = gml::quaternion::eulerAngle<float>(0, angle.x, 0);
			//gml::Quaternion<float> q_pitch = gml::quaternion::eulerAngle<float>(angle.y, 0, 0);

			//gml::Quaternion<float> newRotation = gml::normalize<float>(q_yaw * camera.rotation * q_pitch);
			camera->rotateYaw(angle.x());
			camera->rotatePitch(angle.y());
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
				std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->showCursor();
				mouse.isHidden = false;
			}
			else
			{
				std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->hideCursor();
				mouse.isHidden = true;
			}
		}
	}

private:
	Mouse mouse;
	std::shared_ptr<platform::IWindow> window;
	std::shared_ptr<gl3d::Camera> camera;
};

class App
{
private:
	std::shared_ptr<platform::IWindow> window;
	std::shared_ptr<gl::IContext> glContext;
	InputListener inputListener;

	std::shared_ptr<gl::IShader> shader;
	std::shared_ptr<gl3d::Camera> camera;
	std::shared_ptr<gl::IRenderer> baseRenderer;
	gl3d::SceneRenderer renderer;

	std::shared_ptr<gl3d::Mesh> cubeMesh;
	std::shared_ptr<gl3d::Material> cubeMaterial;

	std::shared_ptr<gl3d::SceneGroup> scene;
	std::shared_ptr<gl3d::SceneGroup> cubeGroup;
	std::shared_ptr<gl3d::SceneObject> light1;
	std::shared_ptr<gl3d::SceneObject> cube1;

public:
	App()
	{
		window = std::make_shared<platform::desktop::GlfwWindow>(1500, 900,"Sandbox");
		glContext = gl::createContext(window);

		camera = std::make_shared<gl3d::Camera>(gl3d::Camera(gml::Vec3f(0.0f, 0.0f, 5.0f),
													   gml::quaternion::eulerAngle<double>(std::numbers::pi_v<float>, 0, 0)));

		inputListener = InputListener(window, camera);
		window->attach(inputListener);

		baseRenderer = glContext->getRenderer();
		baseRenderer->setClearColor(0x008080FFu);
		baseRenderer->enableDepthTest();
		baseRenderer->setViewport(0, 0, 1500, 900);
		renderer = gl3d::SceneRenderer(baseRenderer);


		std::string vertexShader =
#include "gl3d/shaders/singleColorShader.vert"
			;
		std::string fragmentShader =
#include "gl3d/shaders/singleColorShader.frag"
			;

		shader = glContext->getShaderCreator()->createShader(vertexShader, fragmentShader);

		gml::Mat4f proj = gml::matrix::perspective<float>(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
		shader->setUniform("projection", proj);

		cubeMesh = setupMesh();

		setupGraph();
	}

	void setupGraph()
	{
		cubeMaterial = std::make_shared<gl3d::Material>();
		cubeMaterial->addVec3("ambient", gml::Vec3f(0.1));
		cubeMaterial->addVec3("diffuse", gml::Vec3f(0.61f, 0.04f, 0.04f));
		cubeMaterial->addVec3("specular", gml::Vec3f(0.7f, 0.6f, 0.6));
		cubeMaterial->addFloat("shininess", 32.0f);
		cubeMaterial->setShader(shader);


		std::shared_ptr<gl3d::PointLight> lightRes = 
			std::make_shared<gl3d::PointLight>(gl3d::PointLight(
				{ gml::Vec3f(0.2f), gml::Vec3f(0.5f), gml::Vec3f(1.0f) },
				{ 1.0f, 0.09f, 0.032f }));

		cube1 = std::make_shared<gl3d::SceneObject>("cube");
		cube1->bindMaterial(cubeMaterial);
		cube1->bindMesh(cubeMesh);

		light1 = std::make_shared<gl3d::SceneObject>("light");
		light1->bindMaterial(cubeMaterial);
		light1->bindMesh(cubeMesh);
		light1->bindLight(lightRes);
		light1->setTransform(
			gml::matrix::fromQuaternion<double>(gml::quaternion::eulerAngle<double>(0, 0, 0)) *
					gml::matrix::translate<double>(gml::Vec3d(0.0, 0.0, -3.0)) *
					gml::matrix::scale<double>(0.1f, 0.1f, 0.1f));

		scene = std::make_shared<gl3d::SceneGroup>("world");
		cubeGroup = std::make_shared<gl3d::SceneGroup>("cubeGroup");

		scene->addChild(cubeGroup);
		scene->addChild(cube1);
		cubeGroup->addChild(light1);
	}

	void run()
	{
		double coord = -1.0;
		window->show();
		std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

		physics3d::Simulation simulation;
		auto& particle1 = simulation.addRigidBody(1, 1);

		auto particle1Model = std::make_shared<gl3d::SceneObject>("particle");
		particle1Model->bindMesh(cubeMesh);
		particle1Model->bindMaterial(cubeMaterial);
		scene->addChild(particle1Model);

		particle1.applyForce(gml::Vec3d(5, 0, 0), gml::Vec3d(0, -0.5, 0));
		while (!window->shouldDestroy())
		{
			double dt = std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();
			baseRenderer->clear();


			simulation.integrate(1.0/60);
			particle1Model->setTransform(gml::matrix::translate(particle1.getPosition()) * gml::matrix::fromQuaternion(particle1.getOrientation()));

			coord += 0.5 * dt;
			gml::Mat4d localTransform =
				gml::matrix::axisAngle<float>(gml::Vec3d(1.0, 0.0, 0.0), coord) *
				gml::matrix::translate<float>(gml::Vec3d(0.0, 0.0, -3.0)) *
				gml::matrix::scale<float>(0.1f, 0.1f, 0.1f);
			light1->setTransform(localTransform);

			baseRenderer->useShader(*shader);
			shader->setUniform("view", camera->getViewMatrix());
			shader->setUniform("viewPos", camera->getPosition());

			renderer.renderGraph(scene);

			window->swapBuffers();
			window->pollEvents();
		}
	}

	std::shared_ptr<gl3d::Mesh> setupMesh()
	{
		const std::vector<float> vertices = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};

		auto cubeDrawable = glContext->getDrawableCreator()->createDrawable(
			vertices, { 3, 3 }, gl::VertexFormat::INTERLEAVED);

		return std::make_shared<gl3d::Mesh>(std::move(cubeDrawable));
	}
};
