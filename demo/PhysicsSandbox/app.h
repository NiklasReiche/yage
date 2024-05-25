#pragma once

#include "core/platform/Window.h"
#include "core/platform/desktop/GlfwWindow.h"
#include "core/platform/IFileReader.h"
#include "core/platform/desktop/FileReader.h"
#include "core/gl/Context.h"
#include "gml/gml.h"
#include "gl3d/camera.h"
#include "gl3d/sceneRenderer.h"
#include "gl3d/resources/obj.h"
#include "gl3d/resources/gltf.h"
#include "gl3d/light.h"
#include "physics3d/Simulation.h"

#include "MovementListener.h"
#include "ProjectionView.h"
#include "gl3d/shaders.h"

class App
{
public:
	App()
	{
		window = std::make_shared<platform::desktop::GlfwWindow>(1500, 900, "Sandbox");
		glContext = gl::createContext(window);

		camera = std::make_shared<gl3d::Camera>(
			gl3d::Camera(gml::Vec3f(0.0f, 0.0f, 10.0f),
			             gml::quaternion::eulerAngle<double>(std::numbers::pi_v<float>, 0, 0)));

		inputListener = MovementListener(window, camera);
		window->attach(inputListener);

		baseRenderer = glContext->getRenderer();
		baseRenderer->setClearColor(0x008080FFu);
		baseRenderer->enableDepthTest();
		baseRenderer->setViewport(0, 0, 1500, 900);
		renderer = std::make_shared<gl3d::SceneRenderer>(baseRenderer);

		auto fileReader = platform::desktop::FileReader();
		std::string csVertexShader = fileReader.
			openTextFile("shaders/pointShader.vert", platform::IFile::AccessMode::READ)->readAll();
		std::string csFragmentShader = fileReader.
			openTextFile("shaders/pointShader.frag", platform::IFile::AccessMode::READ)->readAll();
		std::string csGeometryShader = fileReader.
			openTextFile("shaders/pointShader.geom", platform::IFile::AccessMode::READ)->readAll();
		csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

        pbrShader = glContext->getShaderCreator()->createShader(gl3d::shaders::PbrShader::vert,
                                                                gl3d::shaders::PbrShader::frag);

		auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
		pbrShader->linkUniformBlock(*ubo);
		csShader->linkUniformBlock(*ubo);

		projViewUniform = ProjectionView(ubo);
		projViewUniform.projection = gml::matrix::perspective<float>(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
		projViewUniform.syncProjection();

        scene = std::make_shared<gl3d::SceneGroup>("world");
		loadResources();
	}


	void run()
	{
		window->show();
		std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

		auto ball1 = loadModel("models/billiard_ball/scene.gltf",
                               physics3d::SphereShape(1, 1),
                               gml::Vec3d(-4, 0, 0));
        ball1->bounding_shape.center = gml::Vec3d(-4, 0, 0);
        ball1->bounding_shape.radius = 1;
        ball1->applyForce(gml::Vec3d(40, 0, 2), gml::Vec3d(0, 0, 0));

        auto ball2 = loadModel("models/billiard_ball/scene.gltf",
                               physics3d::SphereShape(1, 1),
                               gml::Vec3d(4, 0, 0));
        ball2->bounding_shape.center = gml::Vec3d(4, 0, 0);
        ball2->bounding_shape.radius = 1;
        ball2->applyForce(gml::Vec3d(-30, 5, 0), gml::Vec3d(0, 0, 0));

		auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{ },
		                                                             std::vector<unsigned int>{ },
		                                                             { },
		                                                             gl::VertexFormat::INTERLEAVED);

		while (!window->shouldDestroy()) {
			baseRenderer->clear();

			inputListener.applyUpdate();

			simulation.integrate(1.0 / 60);

			updateSceneGraph();

			projViewUniform.view = camera->getViewMatrix();
			projViewUniform.syncView();

			baseRenderer->useShader(*csShader);
			baseRenderer->draw(*point);

			baseRenderer->useShader(*pbrShader);
			pbrShader->setUniform("camPos", camera->getPosition());

			renderer->renderGraph(scene);

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
	std::shared_ptr<gl3d::SceneRenderer> renderer;

	std::shared_ptr<gl3d::SceneGroup> scene;

	physics3d::Simulation simulation;
	std::vector<std::tuple<std::shared_ptr<gl3d::SceneNode>, std::shared_ptr<physics3d::RigidBody>>> objects;

	void updateSceneGraph()
	{
		for (auto& pair : objects) {
			auto& object = std::get<0>(pair);
			auto& rb = std::get<1>(pair);
			object->setTransform(gml::matrix::translate(rb->getPosition()) *
			                     gml::matrix::fromQuaternion(rb->getOrientation()));
		}
	}

	std::shared_ptr<physics3d::RigidBody> loadModel(const std::string& filename, physics3d::InertiaShape shape, gml::Vec3d position)
	{
        auto model = gl3d::resources::readGltf(platform::desktop::FileReader(),
                                               filename, *glContext->getDrawableCreator(),
                                               *glContext->getTextureCreator(), pbrShader);
		scene->addChild(model);

		auto rb = std::make_shared<physics3d::RigidBody>(shape, position, gml::Quatd());
		simulation.addRigidBody(rb);

		objects.emplace_back(model, rb);
		return rb;
	}

	void loadResources()
	{
		auto lightRes = std::make_shared<gl3d::DirectionalLight>();
        lightRes->color = gml::Vec3f(4);
		auto light = std::make_shared<gl3d::SceneObject>("light");
        light->bindLight(lightRes);
        light->setTransform(
			gml::matrix::fromQuaternion<double>(gml::quaternion::eulerAngle<double>(3.14, 0, 0)));
		scene->addChild(light);

        lightRes = std::make_shared<gl3d::DirectionalLight>();
        lightRes->color = gml::Vec3f(2.5);
        light = std::make_shared<gl3d::SceneObject>("light");
        light->bindLight(lightRes);
        light->setTransform(
                gml::matrix::fromQuaternion<double>(gml::quaternion::eulerAngle<double>(0, 0, 0)));
        scene->addChild(light);
	}
};
