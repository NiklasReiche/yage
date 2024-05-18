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
#include <gl3d/shaders.h>

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

		auto fileReader = platform::desktop::FileReader();
		std::string csVertexShader = fileReader.
			openTextFile("assets/shaders/pointShader.vert", platform::IFile::AccessMode::READ)->readAll();
		std::string csFragmentShader = fileReader.
			openTextFile("assets/shaders/pointShader.frag", platform::IFile::AccessMode::READ)->readAll();
		std::string csGeometryShader = fileReader.
			openTextFile("assets/shaders/pointShader.geom", platform::IFile::AccessMode::READ)->readAll();
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
		model = loadModel(filename);
		scene->addChild(model);

		lights.push_back(std::make_shared<gl3d::pbr::PointLight>(gl3d::pbr::PointLight{
			gml::Vec3f(50, 50, 50),
			gml::Vec3f(0, 1, 5)
		}));
#if 0
		lights.push_back(std::make_shared<gl3d::pbr::PointLight>(gl3d::pbr::PointLight{
			gml::Vec3f(100, 100, 100),
			gml::Vec3f(0, 1, 5)
		}));
#endif

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

        baseRenderer->setClearColor(gl::Color::WHITE);

		while (!window->shouldDestroy()) {
			baseRenderer->clear();

			inputListener.applyUpdate();

			projViewUniform.view = camera->getViewMatrix();
			projViewUniform.syncView();

			baseRenderer->useShader(*csShader);
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
		std::shared_ptr<gl3d::Mesh> mesh = gl3d::resources::readGltf(platform::desktop::FileReader(),
		                                       filename, *glContext->getDrawableCreator(), *glContext->getTextureCreator());
        auto mesh_node = std::make_shared<gl3d::SceneObject>(&""[std::rand()]);
        mesh_node->bindMesh(mesh);

        for (auto& sub_mesh : mesh->sub_meshes()) {
            sub_mesh->material().setShader(pbrShader);
        }

		return mesh_node;
	}
};
