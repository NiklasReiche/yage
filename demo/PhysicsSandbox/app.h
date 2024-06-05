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
                gl3d::Camera(gml::Vec3f(0.0f, 2.0f, 2.0f),
                             gml::quaternion::eulerAngle<double>(gml::toRad(180.), 0, 0) *
                             gml::quaternion::eulerAngle<double>(0, 0, gml::toRad(45.))));

        baseRenderer = glContext->getRenderer();
        baseRenderer->setClearColor(0x008080FFu);
        baseRenderer->enableDepthTest();
        baseRenderer->setViewport(0, 0, 1500, 900);
        renderer = std::make_shared<gl3d::SceneRenderer>(baseRenderer);

        auto fileReader = platform::desktop::FileReader();
        std::string csVertexShader = fileReader.openTextFile(
                "shaders/pointShader.vert", platform::IFile::AccessMode::READ)->readAll();
        std::string csFragmentShader = fileReader.openTextFile(
                "shaders/pointShader.frag", platform::IFile::AccessMode::READ)->readAll();
        std::string csGeometryShader = fileReader.openTextFile(
                "shaders/pointShader.geom", platform::IFile::AccessMode::READ)->readAll();
        csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

        pbrShaderNormalMapping = glContext->getShaderCreator()->createShader(
                gl3d::shaders::PbrNormalMappingShader::vert, gl3d::shaders::PbrNormalMappingShader::frag);
        pbrShader = glContext->getShaderCreator()->createShader(
                gl3d::shaders::PbrShader::vert, gl3d::shaders::PbrShader::frag);

        auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
        pbrShaderNormalMapping->linkUniformBlock(*ubo);
        pbrShader->linkUniformBlock(*ubo);
        csShader->linkUniformBlock(*ubo);

        projViewUniform = ProjectionView(ubo);
        projViewUniform.projection = gml::matrix::perspective<float>(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);
        projViewUniform.syncProjection();

        scene = std::make_shared<gl3d::SceneGroup>("world");
        loadResources();

        inputListener = MovementListener(window, camera, &simulation, this);
        window->attach(inputListener);
    }


    void run()
    {
        window->show();
        std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

#if 0
        const double start = 0.35;
        const double epsilon = 0.00000001;
        const double height =
                std::sqrt(-5 * 0.5 * 2 * radius * 5 * 0.5 * 2 * radius + 5 * 2 * radius * 5 * 2 * radius) / 5.;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < i + 1; ++j) {
                auto b = load_ball("models/billiard_ball/scene.gltf",
                                   gml::Vec3d(start + i * height + epsilon * i, 0.031,
                                              -(i * radius * 2) / 2.0 + j * radius * 2 + j * epsilon));
            }
        }
#endif

        auto barrier_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                              "models/barrier.glb", *glContext->getDrawableCreator(),
                                                              *glContext->getTextureCreator(), pbrShader,
                                                              pbrShaderNormalMapping).at(0);

        auto ground_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                             "models/ground.glb", *glContext->getDrawableCreator(),
                                                             *glContext->getTextureCreator(), pbrShader,
                                                             pbrShaderNormalMapping).at(0);

#if 0
        auto ground = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::BoundingVolume{physics3d::BPlane{
                        .original_normal = {0, -1, 0},
                }},
                billiard_table,
                gml::Vec3d(0, 0, 0),
                gml::quaternion::eulerAngle<double>(0, 0, 0));

        auto& scene_ground = scene->create_object("ground");
        scene_ground.mesh = ground_mesh;

        objects.emplace_back(scene_ground, ground);

        auto barrier1 = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::BoundingVolume{physics3d::BPlane{
                        .original_normal = {0, 0, -1},
                }},
                billiard_table,
                gml::Vec3d(1, 0, 0),
                gml::quaternion::eulerAngle<double>(-std::numbers::pi_v<double> / 2, 0, 0));
        auto& scene_barrier1 = scene->create_object("barrier1");
        scene_barrier1.mesh = barrier_mesh;
        objects.emplace_back(scene_barrier1, barrier1);

        auto barrier2 = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::BoundingVolume{physics3d::BPlane{
                        .original_normal = {0, 0, -1},
                }},
                billiard_table,
                gml::Vec3d(0, 0, 1),
                gml::quaternion::eulerAngle<double>(std::numbers::pi_v<double>, 0, 0));
        auto& scene_barrier2 = scene->create_object("barrier2");
        scene_barrier2.mesh = barrier_mesh;
        objects.emplace_back(scene_barrier2, barrier2);

        auto barrier3 = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::BoundingVolume{physics3d::BPlane{
                        .original_normal = {0, 0, -1},
                }},
                billiard_table,
                gml::Vec3d(0, 0, -1),
                gml::Quatd());
        auto& scene_barrier3 = scene->create_object("barrier3");
        scene_barrier3.mesh = barrier_mesh;
        objects.emplace_back(scene_barrier3, barrier3);

        auto barrier4 = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::BoundingVolume{physics3d::BPlane{}},
                billiard_table,
                gml::Vec3d(-1, 0, 0),
                gml::quaternion::eulerAngle<double>(std::numbers::pi_v<double> / 2, 0, 0));
        auto& scene_barrier4 = scene->create_object("barrier4");
        scene_barrier4.mesh = barrier_mesh;
        objects.emplace_back(scene_barrier4, barrier4);
#endif

        auto ball = load_ball("models/billiard_ball/scene.gltf",
                              gml::Vec3d(-0.5, 0.031, 0));
        inputListener.ball = ball.get();

        auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{},
                                                                     std::vector<unsigned int>{},
                                                                     {},
                                                                     gl::VertexFormat::INTERLEAVED);

        load_cube("models/box.glb", gml::Vec3d(0,1,0), physics3d::InertiaShape::static_shape());
        load_cube("models/box.glb", gml::Vec3d(0.5, 4, 0), physics3d::InertiaShape::cube(2, 1),
                  gml::quaternion::eulerAngle<double>(0.0, gml::toRad(10.0), gml::toRad(40.0)));

        while (!window->shouldDestroy()) {
            baseRenderer->clear();

            inputListener.applyUpdate();

            if (simulate) {
                simulation.update(1.0 / 60);
            }

            updateSceneGraph();

            projViewUniform.view = camera->getViewMatrix();
            projViewUniform.syncView();

            baseRenderer->useShader(*csShader);
            baseRenderer->draw(*point);

            pbrShaderNormalMapping->setUniform("camPos", camera->getPosition());
            pbrShader->setUniform("camPos", camera->getPosition());

            renderer->renderGraph(scene);

            window->swapBuffers();
            window->pollEvents();
        }
    }

public:
    bool simulate = false;

private:
    const double radius = 0.03;
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
    std::vector<std::tuple<std::reference_wrapper<gl3d::SceneNode>, std::shared_ptr<physics3d::RigidBody>>> objects;

    std::shared_ptr<gl3d::Mesh> ball_mesh;
    std::shared_ptr<gl3d::Mesh> cube_mesh;

    physics3d::Material billiard_ball{
            .restitution = 0.99,
            .kinetic_friction = 0.2,
            .rolling_friction = 0.001,
    };

    physics3d::Material billiard_table{
            .restitution = 0.9,
            .kinetic_friction = 1.0,
            .rolling_friction = 1.0,
    };

    physics3d::Material cube_material{
            .restitution = 0.0,
            .kinetic_friction = 0.5,
    };


    void updateSceneGraph()
    {
        for (auto& [object, rb]: objects) {
            object.get().local_transform = gml::matrix::translate(rb->position()) *
                                           gml::matrix::fromQuaternion(rb->orientation()) *
                                           gml::matrix::scale(object.get().local_transform.getScale());
        }
    }

    std::shared_ptr<physics3d::RigidBody>
    load_ball(const std::string& filename, gml::Vec3d position)
    {
        if (!ball_mesh) {
            ball_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                          filename, *glContext->getDrawableCreator(),
                                                          *glContext->getTextureCreator(), pbrShader,
                                                          pbrShaderNormalMapping).at(0);
        }

        auto& scene_object = scene->create_object("ball");
        scene_object.local_transform = gml::matrix::scale<double>(gml::Vec3d(radius));
        scene_object.mesh = ball_mesh;

        auto rb = simulation.create_rigid_body(
                physics3d::InertiaShape::sphere(radius, mass),
                physics3d::BoundingVolume{physics3d::BSphere{
                        .radius = radius,
                }},
                billiard_ball,
                position,
                gml::quaternion::fromMatrix(
                        gml::matrix::axisAngle(gml::Vec3d(0, 0, 1), std::numbers::pi_v<double> / 2).getRotation()));

        objects.emplace_back(scene_object, rb);
        return rb;
    }

    std::shared_ptr<physics3d::RigidBody>
    load_cube(const std::string& filename, gml::Vec3d position, physics3d::InertiaShape shape, gml::Quatd orientation = gml::Quatd())
    {
        if (!cube_mesh) {
            cube_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                          filename, *glContext->getDrawableCreator(),
                                                          *glContext->getTextureCreator(), pbrShader,
                                                          pbrShaderNormalMapping).at(0);
        }

        auto& scene_object = scene->create_object("cube");
        scene_object.mesh = cube_mesh;

        auto rb = simulation.create_rigid_body(
                shape,
                physics3d::BoundingVolume{physics3d::BOrientedBox{
                        .half_size = gml::Vec3d(1, 1, 1),
                }},
                cube_material,
                position,
                orientation);

        objects.emplace_back(scene_object, rb);
        return rb;
    }

    void loadResources()
    {
        auto lightRes = std::make_shared<gl3d::DirectionalLight>();
        lightRes->color = gml::Vec3f(3.5);

        auto& light = scene->create_object("light");
        light.light = lightRes;
        light.local_transform =
                gml::matrix::fromQuaternion<double>(
                        gml::quaternion::eulerAngle<double>(gml::toRad(165.), 0, 0) *
                        gml::quaternion::eulerAngle<double>(0, 0, gml::toRad(75.))
                );
    }
};
