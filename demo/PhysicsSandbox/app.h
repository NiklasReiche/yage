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
                gl3d::Camera(gml::Vec3f(0.0f, 6.0f, 10.0f),
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
                                                       openTextFile("shaders/pointShader.vert",
                                                                    platform::IFile::AccessMode::READ)->readAll();
        std::string csFragmentShader = fileReader.
                                                         openTextFile("shaders/pointShader.frag",
                                                                      platform::IFile::AccessMode::READ)->readAll();
        std::string csGeometryShader = fileReader.
                                                         openTextFile("shaders/pointShader.geom",
                                                                      platform::IFile::AccessMode::READ)->readAll();
        csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

        pbrShaderNormalMapping = glContext->getShaderCreator()
                                          ->createShader(gl3d::shaders::PbrNormalMappingShader::vert,
                                                         gl3d::shaders::PbrNormalMappingShader::frag);
        pbrShader = glContext->getShaderCreator()->createShader(gl3d::shaders::PbrShader::vert,
                                                                gl3d::shaders::PbrShader::frag);

        auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
        pbrShaderNormalMapping->linkUniformBlock(*ubo);
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

        const double epsilon = 0.00000001;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < i + 1; ++j) {
                auto b = loadModel("models/billiard_ball/scene.gltf",
                                   physics3d::SphereShape(radius, mass),
                                   gml::Vec3d(i * std::sqrt(radius * 2) + epsilon * i, 0,
                                              -(i * radius * 2) / 2.0 + j * radius * 2 + j * epsilon));
            }
        }

        inputListener.ball = loadModel("models/billiard_ball/scene.gltf",
                                       physics3d::SphereShape(radius, mass),
                                       gml::Vec3d(-10, 0, 0));

        auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{},
                                                                     std::vector<unsigned int>{},
                                                                     {},
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

            baseRenderer->useShader(*pbrShaderNormalMapping);
            pbrShaderNormalMapping->setUniform("camPos", camera->getPosition());

            renderer->renderGraph(scene);

            window->swapBuffers();
            window->pollEvents();
        }
    }

private:
    const double radius = 0.6;
    const double mass = 0.17;

    std::shared_ptr<platform::IWindow> window;
    std::shared_ptr<gl::IContext> glContext;
    MovementListener inputListener;

    ProjectionView projViewUniform;

    std::shared_ptr<gl::IShader> shader;
    std::shared_ptr<gl::IShader> csShader;
    std::shared_ptr<gl::IShader> pbrShaderNormalMapping;
    std::shared_ptr<gl::IShader> pbrShader;
    std::shared_ptr<gl3d::Camera> camera;
    std::shared_ptr<gl::IRenderer> baseRenderer;
    std::shared_ptr<gl3d::SceneRenderer> renderer;

    std::shared_ptr<gl3d::SceneGroup> scene;

    physics3d::Simulation simulation;
    std::vector<std::tuple<std::shared_ptr<gl3d::SceneNode>, std::shared_ptr<physics3d::RigidBody>>> objects;

    std::shared_ptr<gl3d::Mesh> ball_mesh;


    void updateSceneGraph()
    {
        for (auto& pair: objects) {
            auto& object = std::get<0>(pair);
            auto& rb = std::get<1>(pair);
            object->setTransform(
                    gml::matrix::translate(rb->getPosition()) *
                    gml::matrix::fromQuaternion(rb->getOrientation()) *
                    gml::matrix::scale(object->getTransform().getScale())
            );
        }
    }

    std::shared_ptr<physics3d::RigidBody>
    loadModel(const std::string& filename, physics3d::InertiaShape shape, gml::Vec3d position)
    {
        if (!ball_mesh) {
            ball_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                          filename, *glContext->getDrawableCreator(),
                                                          *glContext->getTextureCreator(), pbrShader,
                                                          pbrShaderNormalMapping).at(0);
        }

        auto scene_object = std::make_shared<gl3d::SceneObject>();
        scene_object->setTransform(gml::matrix::scale<double>(gml::Vec3d(radius)));
        scene->addChild(scene_object);
        scene_object->bindMesh(ball_mesh);

        auto rb = std::make_shared<physics3d::RigidBody>(
                shape,
                physics3d::BoundingVolume{physics3d::BSphere{
                        .radius = radius,
                }},
                position,
                gml::quaternion::fromMatrix(
                        gml::matrix::axisAngle(gml::Vec3d(0, 0, 1), std::numbers::pi_v<double> / 2).getRotation()),
                gml::Vec3d(0));
        simulation.addRigidBody(rb);

        objects.emplace_back(scene_object, rb);
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
