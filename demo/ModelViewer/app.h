#pragma once

#include <core/platform/Window.h>
#include <core/platform/desktop/GlfwWindow.h>
#include <core/platform/IFileReader.h>
#include <core/platform/desktop/FileReader.h>
#include <core/gl/Context.h>
#include <math/math.h>
#include <gl3d/camera.h>
#include <gl3d/sceneRenderer.h>
#include <gl3d/resources/obj.h>
#include <gl3d/resources/gltf.h>
#include <gl3d/shaders.h>

#include "MovementListener.h"
#include "ProjectionView.h"

using namespace yage;

class App
{
public:
    explicit App(const std::string& filename)
    {
        window = std::make_shared<platform::desktop::GlfwWindow>(1500, 900, "Sandbox");
        glContext = gl::createContext(window);

        camera = std::make_shared<gl3d::Camera>();

        baseRenderer = glContext->getRenderer();
        baseRenderer->enableDepthTest();
        baseRenderer->setViewport(0, 0, 1500, 900);
        renderer = std::make_unique<gl3d::SceneRenderer>(*glContext);

        const auto fileReader = platform::desktop::FileReader();
        const std::string csVertexShader = fileReader.openTextFile("assets/shaders/pointShader.vert",
                                                                   platform::IFile::AccessMode::READ)->readAll();
        const std::string csFragmentShader = fileReader.openTextFile("assets/shaders/pointShader.frag",
                                                                     platform::IFile::AccessMode::READ)->readAll();
        const std::string csGeometryShader = fileReader.openTextFile("assets/shaders/pointShader.geom",
                                                                     platform::IFile::AccessMode::READ)->readAll();
        csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

        pbrShader = glContext->getShaderCreator()->createShader(gl3d::shaders::PbrShader::vert,
                                                                gl3d::shaders::PbrShader::frag);
        pbrShaderNormalMapping = glContext->getShaderCreator()->createShader(gl3d::shaders::PbrNormalMappingShader::vert,
                                                                gl3d::shaders::PbrNormalMappingShader::frag);

        auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
        csShader->linkUniformBlock(renderer->projection_view().ubo());

        projViewUniform = ProjectionView(ubo);
        projViewUniform.projection = math::matrix::perspective<float>(90.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
        projViewUniform.syncProjection();
        projViewUniform.view = math::matrix::look_at<float>(math::Vec3f(5, 5, 5), math::Vec3f(0, 0, 0));
        projViewUniform.syncView();
        pbrShader->setUniform("camPos", math::Vec3f(5, 5, 5));
        pbrShaderNormalMapping->setUniform("camPos", math::Vec3f(5, 5, 5));

        scene = loadScene(filename);

        auto light = std::make_shared<gl3d::DirectionalLight>();
        light->color = math::Vec3f(5, 5, 5);
        auto& light_node = scene->create_object("light");
        light_node.local_transform = math::matrix::from_quaternion<double>(
                math::quaternion::euler_angle<double>(math::to_rad(180.0), math::to_rad(0.0), math::to_rad(45.0)));
        light_node.light = light;

        inputListener = MovementListener(window, camera);
        inputListener.pbrShader = pbrShader;
        inputListener.pbrShader2 = pbrShaderNormalMapping;
        inputListener.m_projection_view = &projViewUniform;
        inputListener.world = scene;
        inputListener.camPos = math::Vec3f(5);
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

            renderer->render_graph(scene, *camera);

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
    std::shared_ptr<gl::IShader> pbrShaderNormalMapping;
    std::shared_ptr<gl3d::Camera> camera;
    std::shared_ptr<gl::IRenderer> baseRenderer;
    std::unique_ptr<gl3d::SceneRenderer> renderer;

    std::shared_ptr<gl3d::SceneGroup> scene;

    std::shared_ptr<gl3d::SceneGroup> loadScene(const std::string& filename)
    {
        return gl3d::resources::gltf_read_scene(platform::desktop::FileReader(),
                                                filename, *glContext->getDrawableCreator(),
                                                *glContext->getTextureCreator(), pbrShader, pbrShaderNormalMapping);
    }
};
