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

#include "MovementListener.h"
#include "ProjectionView.h"

class App
{
public:
	explicit App(const std::string& filename)
	{
		window = std::make_shared<platform::desktop::GlfwWindow>(1500, 900, "Sandbox");
		glContext = gl::createContext(window);

		camera = std::make_shared<gl3d::Camera>(
			gl3d::Camera(gml::Vec3f(0.0f, 0.0f, 5.0f),
			             gml::quaternion::eulerAngle<double>(std::numbers::pi_v<float>, 0, 0)));

		baseRenderer = glContext->getRenderer();
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
			openTextFile("assets/shaders/pointShader.vert", platform::IFile::AccessMode::READ)->readAll();
		std::string csFragmentShader = fileReader.
			openTextFile("assets/shaders/pointShader.frag", platform::IFile::AccessMode::READ)->readAll();
		std::string csGeometryShader = fileReader.
			openTextFile("assets/shaders/pointShader.geom", platform::IFile::AccessMode::READ)->readAll();
		csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

		std::string pbrVertexShader =
#include "gl3d/shaders/pbrShader.vert"
	;
		std::string pbrFragmentShader =
#include "gl3d/shaders/pbrShader.frag"
	;
		pbrShader = glContext->getShaderCreator()->createShader(pbrVertexShader, pbrFragmentShader);

		auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
		pbrShader->linkUniformBlock(*ubo);
		csShader->linkUniformBlock(*ubo);

		projViewUniform = ProjectionView(ubo);
		projViewUniform.projection = gml::matrix::perspective<float>(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
		projViewUniform.syncProjection();

		scene = std::make_shared<gl3d::SceneGroup>("world");
		model = loadModel(filename);
		scene->addChild(model);

		lights.push_back(std::make_shared<gl3d::pbr::PointLight>(gl3d::pbr::PointLight{
			gml::Vec3f(100, 100, 100),
			gml::Vec3f(0, 1, 5)
		}));
		lights.push_back(std::make_shared<gl3d::pbr::PointLight>(gl3d::pbr::PointLight{
			gml::Vec3f(100, 100, 100),
			gml::Vec3f(0, 1, 5)
		}));

		inputListener = MovementListener(window, camera, lights);
		window->attach(inputListener);
	}


	void run()
	{
		window->show();
		std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

		auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{ },
		                                                             std::vector<unsigned int>{ },
		                                                             {  },
		                                                             gl::VertexFormat::INTERLEAVED);

		while (!window->shouldDestroy()) {
			baseRenderer->clear();

			inputListener.applyUpdate();

			projViewUniform.view = camera->getViewMatrix();
			projViewUniform.syncView();

			baseRenderer->useShader(*csShader);
			csShader->setUniform("viewPos", camera->getPosition());
			baseRenderer->draw(*point);

			baseRenderer->useShader(*pbrShader);
			pbrShader->setUniform("camPos", camera->getPosition());
			pbrShader->setUniform("n_pointLights", (int)lights.size());
			for (int i = 0; i < (int)lights.size(); ++i) {
				pbrShader->setUniform("pointLights[" + utils::toString(i) + "].position", lights[i]->position);
				pbrShader->setUniform("pointLights[" + utils::toString(i) + "].color", lights[i]->color);
			}

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
	std::shared_ptr<gl3d::SceneObject> model;
	std::vector<std::shared_ptr<gl3d::pbr::PointLight>> lights;


	std::shared_ptr<gl3d::SceneObject> loadModel(const std::string& filename)
	{
		auto tuple = gl3d::resources::readGltf(platform::desktop::FileReader(),
		                                       filename, *glContext->getDrawableCreator());
		auto mesh = std::make_shared<gl3d::Mesh>(std::move(std::get<0>(tuple)));
		auto material = std::make_shared<gl3d::Material>(std::get<1>(tuple));
		material->setShader(pbrShader);

		auto object = std::make_shared<gl3d::SceneObject>(&""[std::rand()]);
		object->bindMaterial(material);
		object->bindMesh(mesh);

		return object;
	}
};