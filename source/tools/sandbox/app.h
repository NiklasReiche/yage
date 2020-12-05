#pragma once

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/gl/Context.h>
#include <gml/gml.h>
#include <gl3d/camera.h>
#include <gl3d/sceneRenderer.h>
#include <gl3d/resourceManager.h>
#include <physics3d/Simulation.h>

#include "InputListener.h"

class App
{
public:
	App()
	{
		window = std::make_shared<platform::desktop::GlfwWindow>(1500, 900, "Sandbox");
		glContext = gl::createContext(window);

		camera = std::make_shared<gl3d::Camera>(
			gl3d::Camera(gml::Vec3f(0.0f, 0.0f, 5.0f),
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

		std::string csVertexShader =
#include "shaders/pointShader.vert"
	;
		std::string csFragmentShader =
#include "shaders/pointShader.frag"
	;
		std::string csGeometryShader =
#include "shaders/pointShader.geom"
		;
		csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

		gml::Mat4f proj = gml::matrix::perspective<float>(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
		shader->setUniform("projection", proj);
		csShader->setUniform("projection", proj);

		loadResources();
	}


	void run()
	{
		window->show();
		std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

		auto cube1 = createCube();
		cube1->applyForce(gml::Vec3d(0, 10, 0), gml::Vec3d(0.3, -0.5, 0));

		auto cube2 = createCube();
		cube2->applyForce(gml::Vec3d(-30, 1, 10), gml::Vec3d(0.5, 0, 0.5));

		auto cube3 = createCube();
		cube3->applyForce(gml::Vec3d(3, -1, 0), gml::Vec3d(0, -0.5, 0));

		auto point = glContext->getDrawableCreator()->createDrawable({}, {}, gl::VertexFormat::BATCHED);

		while (!window->shouldDestroy()) {
			baseRenderer->clear();

			simulation.integrate(1.0 / 60);
			updateSceneGraph();

			baseRenderer->useShader(*csShader);
			csShader->setUniform("view", camera->getViewMatrix());
			csShader->setUniform("viewPos", camera->getPosition());
			baseRenderer->draw(*point);

			baseRenderer->useShader(*shader);
			shader->setUniform("view", camera->getViewMatrix());
			shader->setUniform("viewPos", camera->getPosition());

			renderer.renderGraph(scene);

			window->swapBuffers();
			window->pollEvents();
		}
	}

private:
	std::shared_ptr<platform::IWindow> window;
	std::shared_ptr<gl::IContext> glContext;
	InputListener inputListener;

	std::shared_ptr<gl::IShader> shader;
	std::shared_ptr<gl::IShader> csShader;
	std::shared_ptr<gl3d::Camera> camera;
	std::shared_ptr<gl::IRenderer> baseRenderer;
	gl3d::SceneRenderer renderer;

	std::shared_ptr<gl3d::Mesh> cubeMesh;
	std::shared_ptr<gl3d::Material> cubeMaterial;

	std::shared_ptr<gl3d::SceneGroup> scene;
	std::shared_ptr<gl3d::SceneObject> light1;

	physics3d::Simulation simulation;
	std::vector<std::tuple<std::shared_ptr<gl3d::SceneObject>, std::shared_ptr<physics3d::RigidBody>>> objects;

	void updateSceneGraph()
	{
		for (auto& pair : objects) {
			auto& model = std::get<0>(pair);
			auto& rb = std::get<1>(pair);
			model->setTransform(gml::matrix::translate(rb->getPosition()) *
			                    gml::matrix::fromQuaternion(rb->getOrientation()));
		}
	}

	std::shared_ptr<physics3d::RigidBody> createCube()
	{
		auto model = std::make_shared<gl3d::SceneObject>(&""[std::rand()]);
		model->bindMaterial(cubeMaterial);
		model->bindMesh(cubeMesh);
		scene->addChild(model);

		auto rb = std::make_shared<physics3d::RigidBody>(1, 1);
		simulation.addRigidBody(rb);

		objects.emplace_back(model, rb);
		return rb;
	}

	void loadResources()
	{
		cubeMesh = setupMesh();

		cubeMaterial = std::make_shared<gl3d::Material>();
		cubeMaterial->addVec3("ambient", gml::Vec3f(0.1));
		cubeMaterial->addVec3("diffuse", gml::Vec3f(0.61f, 0.04f, 0.04f));
		cubeMaterial->addVec3("specular", gml::Vec3f(0.7f, 0.6f, 0.6));
		cubeMaterial->addFloat("shininess", 32.0f);
		cubeMaterial->setShader(shader);

		scene = std::make_shared<gl3d::SceneGroup>("world");

		auto lightRes = std::make_shared<gl3d::PointLight>(
			gl3d::PointLight({ gml::Vec3f(0.2f),
			                   gml::Vec3f(0.5f),
			                   gml::Vec3f(1.0f) },
			                 { 1.0f, 0.09f, 0.032f }));
		light1 = std::make_shared<gl3d::SceneObject>("light");
		light1->bindMaterial(cubeMaterial);
		light1->bindMesh(cubeMesh);
		light1->bindLight(lightRes);
		light1->setTransform(
			gml::matrix::fromQuaternion<double>(gml::quaternion::eulerAngle<double>(0, 0, 0)) *
			gml::matrix::translate<double>(gml::Vec3d(0.0, 0.0, -3.0)) *
			gml::matrix::scale<double>(0.1f, 0.1f, 0.1f));
		scene->addChild(light1);
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
