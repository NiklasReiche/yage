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

        camera = std::make_shared<gl3d::Camera>();
        camera->lookAt(gml::Vec3d(5, 5, 5), gml::Vec3d(0, 0, 0));

        baseRenderer = glContext->getRenderer();
        baseRenderer->enableDepthTest();
        baseRenderer->setViewport(0, 0, 1500, 900);
        renderer = std::make_unique<gl3d::SceneRenderer>(baseRenderer);

        auto fileReader = platform::desktop::FileReader();
        std::string csVertexShader = fileReader.openTextFile("assets/shaders/pointShader.vert",
                                                             platform::IFile::AccessMode::READ)->readAll();
        std::string csFragmentShader = fileReader.openTextFile("assets/shaders/pointShader.frag",
                                                               platform::IFile::AccessMode::READ)->readAll();
        std::string csGeometryShader = fileReader.openTextFile("assets/shaders/pointShader.geom",
                                                               platform::IFile::AccessMode::READ)->readAll();
        csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

        pbrShader = glContext->getShaderCreator()->createShader(gl3d::shaders::PbrShader::vert,
                                                                gl3d::shaders::PbrShader::frag);

        auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
        pbrShader->linkUniformBlock(*ubo);
        csShader->linkUniformBlock(*ubo);

        projViewUniform = ProjectionView(ubo);
        projViewUniform.projection = gml::matrix::perspective<float>(90.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
        projViewUniform.syncProjection();
        projViewUniform.view = gml::matrix::lookAt<double>(gml::Vec3d(5, 5, 5), gml::Vec3d(0, 0, 0));
        projViewUniform.syncView();
        pbrShader->setUniform("camPos", camera->getPosition());

        scene = loadScene(filename);

#if 0
        auto light = std::make_shared<gl3d::PointLight>();
        light->color = gml::Vec3f(50, 50, 50);

        auto light_node = std::make_shared<gl3d::SceneObject>();
        light_node->setTransform(gml::matrix::translate(2, 2, 3));
        light_node->bindLight(light);
        scene->addChild(light_node);
#endif

        auto light_2 = std::make_shared<gl3d::DirectionalLight>();
        light_2->color = gml::Vec3f(5, 5, 5);

        auto light_node_2 = std::make_shared<gl3d::SceneObject>();
        light_node_2->setTransform(gml::matrix::fromQuaternion<double>(
                gml::quaternion::eulerAngle<double>(gml::toRad(180.0), gml::toRad(0.0), gml::toRad(0.0))));
        light_node_2->bindLight(light_2);
        scene->addChild(light_node_2);

        inputListener = MovementListener(window, camera);
        inputListener.pbrShader = pbrShader;
        inputListener.m_projection_view = &projViewUniform;
        inputListener.world = scene;
        inputListener.camPos = gml::Vec3f(5);
        window->attach(inputListener);
    }


    void run()
    {
        window->show();
        std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

        auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{},
                                                                     std::vector<unsigned int>{},
                                                                     {},
                                                                     gl::VertexFormat::INTERLEAVED);

        baseRenderer->setClearColor(gl::Color::WHITE);

        while (!window->shouldDestroy()) {
            baseRenderer->clear();

            inputListener.applyUpdate();

            baseRenderer->useShader(*csShader);
            baseRenderer->draw(*point);

            baseRenderer->useShader(*pbrShader);

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

    std::shared_ptr<gl::IShader> csShader;
    std::shared_ptr<gl::IShader> pbrShader;
    std::shared_ptr<gl3d::Camera> camera;
    std::shared_ptr<gl::IRenderer> baseRenderer;
    std::unique_ptr<gl3d::SceneRenderer> renderer;

    std::shared_ptr<gl3d::SceneGroup> scene;

    std::shared_ptr<gl3d::SceneGroup> loadScene(const std::string& filename)
    {
        return gl3d::resources::readGltf(platform::desktop::FileReader(),
                                                                     filename, *glContext->getDrawableCreator(),
                                                                     *glContext->getTextureCreator(), pbrShader);
    }
};
