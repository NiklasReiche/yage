#pragma once

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/platform/IFileReader.h>
#include <core/platform/desktop/FileReader.h>
#include <core/gl/Context.h>
#include <gml/gml.h>
#include <gl3d/camera.h>
#include <gl3d/sceneRenderer.h>
#include <gl3d/resources/obj.h>
#include <gl3d/resources/gltf.h>
#include <physics3d/Simulation.h>

#include "MovementListener.h"
#include "ProjectionView.h"

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

		inputListener = MovementListener(window, camera);
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


		auto fileReader = platform::desktop::FileReader();
		std::string csVertexShader = fileReader.
			openTextFile("shaders/pointShader.vert", platform::IFile::AccessMode::READ)->readAll();
		std::string csFragmentShader = fileReader.
			openTextFile("shaders/pointShader.frag", platform::IFile::AccessMode::READ)->readAll();
		std::string csGeometryShader = fileReader.
			openTextFile("shaders/pointShader.geom", platform::IFile::AccessMode::READ)->readAll();
		csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

		std::string pbrVertexShader = fileReader.
			openTextFile("shaders/pbrShader.vert", platform::IFile::AccessMode::READ)->readAll();
		std::string pbrFragmentShader = fileReader.
			openTextFile("shaders/pbrShader.frag", platform::IFile::AccessMode::READ)->readAll();
		pbrShader = glContext->getShaderCreator()->createShader(pbrVertexShader, pbrFragmentShader);

		auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
		pbrShader->linkUniformBlock(*ubo);
		csShader->linkUniformBlock(*ubo);

		projViewUniform = ProjectionView(ubo);
		projViewUniform.projection = gml::matrix::perspective<float>(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
		projViewUniform.syncProjection();

		loadResources();
	}


	void run()
	{
		window->show();
		std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

		auto cube1 = loadModel("models/sphere.gltf");
		//cube1->applyForce(gml::Vec3d(0, 10, 0), gml::Vec3d(0.3, -0.5, 0));

		//auto cube2 = loadModel("models/cube.gltf");
		//cube2->applyForce(gml::Vec3d(-30, 1, 10), gml::Vec3d(0.5, 0, 0.5));

		//auto cube3 = loadModel("models/cube.gltf");
		//cube3->applyForce(gml::Vec3d(3, -1, 0), gml::Vec3d(0, -0.5, 0));

		auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{ },
		                                                             std::vector<unsigned int>{ },
		                                                             {  },
		                                                             gl::VertexFormat::INTERLEAVED);

		while (!window->shouldDestroy()) {
			baseRenderer->clear();

			inputListener.applyUpdate();

			simulation.integrate(1.0 / 60);

			updateSceneGraph();

			projViewUniform.view = camera->getViewMatrix();
			projViewUniform.syncView();

			baseRenderer->useShader(*csShader);
			csShader->setUniform("viewPos", camera->getPosition());
			baseRenderer->draw(*point);

			baseRenderer->useShader(*pbrShader);
			pbrShader->setUniform("camPos", camera->getPosition());
			pbrShader->setUniform("n_pointLights", 1);
			pbrShader->setUniform("pointLights[0].position", gml::Vec3f(0, 1, 5));
			pbrShader->setUniform("pointLights[0].color", gml::Vec3f(50, 50, 100));

			renderer.renderGraph(scene);

			window->swapBuffers();
			window->pollEvents();
		}
	}

private:
	std::shared_ptr<platform::IWindow> window;
	std::shared_ptr<gl::IContext> glContext;
	MovementListener inputListener;

	ProjectionView projViewUniform;

	std::shared_ptr<gl::IShader> shader;
	std::shared_ptr<gl::IShader> csShader;
	std::shared_ptr<gl::IShader> pbrShader;
	std::shared_ptr<gl3d::Camera> camera;
	std::shared_ptr<gl::IRenderer> baseRenderer;
	gl3d::SceneRenderer renderer;

	std::shared_ptr<gl3d::SceneGroup> scene;
	std::shared_ptr<gl3d::SceneObject> light1;

	physics3d::Simulation simulation;
	std::vector<std::tuple<std::shared_ptr<gl3d::SceneObject>, std::shared_ptr<physics3d::RigidBody>>> objects;

	void updateSceneGraph()
	{
		for (auto& pair : objects) {
			auto& object = std::get<0>(pair);
			auto& rb = std::get<1>(pair);
			object->setTransform(gml::matrix::translate(rb->getPosition()) *
			                     gml::matrix::fromQuaternion(rb->getOrientation()));
		}
	}

	std::shared_ptr<physics3d::RigidBody> loadModel(const std::string& filename)
	{
		auto tuple = gl3d::resources::readGltf(platform::desktop::FileReader(),
		                                       filename, *glContext->getDrawableCreator());
		auto mesh = std::make_shared<gl3d::Mesh>(std::move(std::get<0>(tuple)));
		auto material = std::make_shared<gl3d::Material>(std::get<1>(tuple));
		material->setShader(pbrShader);

		auto object = std::make_shared<gl3d::SceneObject>(&""[std::rand()]);
		object->bindMaterial(material);
		object->bindMesh(mesh);
		scene->addChild(object);

		auto rb = std::make_shared<physics3d::RigidBody>(1, 1);
		simulation.addRigidBody(rb);

		objects.emplace_back(object, rb);
		return rb;
	}

	void loadResources()
	{
		scene = std::make_shared<gl3d::SceneGroup>("world");

		auto lightRes = std::make_shared<gl3d::DirLight>(
			gl3d::DirLight({ gml::Vec3f(0.2f),
			                 gml::Vec3f(0.5f),
			                 gml::Vec3f(1.0f) },
			               gml::Vec3f(-1, -1, -1)));
		light1 = std::make_shared<gl3d::SceneObject>("light");
		light1->bindLight(lightRes);
		light1->setTransform(
			gml::matrix::fromQuaternion<double>(gml::quaternion::eulerAngle<double>(0, 0, 0)) *
			gml::matrix::translate<double>(gml::Vec3d(0.0, 0.0, -3.0)) *
			gml::matrix::scale<double>(0.1f, 0.1f, 0.1f));

		auto lightRes2 = std::make_shared<gl3d::PointLight>(
			gl3d::PointLight(
				{ gml::Vec3f(1), gml::Vec3f(1), gml::Vec3f(1) },
				{ 1.0f, 0.09f, 0.032f }));
		auto light2 = std::make_shared<gl3d::SceneObject>("light2");
		light2->bindLight(lightRes2);

		auto tuple = gl3d::resources::readGltf(platform::desktop::FileReader(),
		                                       "models/cube.gltf", *glContext->getDrawableCreator());
		auto mesh = std::make_shared<gl3d::Mesh>(std::move(std::get<0>(tuple)));
		auto material = std::make_shared<gl3d::Material>(std::get<1>(tuple));
		material->setShader(pbrShader);
		light2->bindMesh(mesh);
		light2->bindMaterial(material);
		light2->setTransform(gml::matrix::translate<double>(gml::Vec3d(0, 1, 5)) *
			                     gml::matrix::scale<double>(0.1, 0.1, 0.1));

		scene->addChild(light2);
	}
};
