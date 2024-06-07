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

        simulation = physics3d::Simulation(physics3d::Visualizer(*glContext));
        simulation.enable_gravity();

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
        setup_lights();

        inputListener = MovementListener(window, camera, &simulation, this);
        window->attach(inputListener);
    }


    void run()
    {
        window->show();
        std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();


        auto ground = load_ground(gml::Vec3d(0));

        load_barrier(gml::Vec3d(1, 0, 0),
                     gml::quaternion::eulerAngle<double>(-std::numbers::pi_v<double> / 2, 0, 0),
                     gml::Vec3d(0.5, 1.0, 1.0));

        load_barrier(gml::Vec3d(0, 0, 0.5),
                     gml::quaternion::eulerAngle<double>(std::numbers::pi_v<double>, 0, 0));

        load_barrier(gml::Vec3d(0, 0, -0.5), gml::Quatd());

        load_barrier(gml::Vec3d(-1, 0, 0),
                     gml::quaternion::eulerAngle<double>(std::numbers::pi_v<double> / 2, 0, 0),
                     gml::Vec3d(0.5, 1.0, 1.0));

        load_balls(gml::Vec3d(0.35, ground->position().y() + billiard_ball_radius + 0.00001, 0));

        auto ball = load_ball("models/old_billiard_ball.glb",
                              gml::Vec3d(-0.5, ground->position().y() + billiard_ball_radius + 0.00001, 0));
        inputListener.ball = ball.get();


        auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{},
                                                                     std::vector<unsigned int>{},
                                                                     {},
                                                                     gl::VertexFormat::INTERLEAVED);

        while (!window->shouldDestroy()) {
            baseRenderer->clear();

            inputListener.applyUpdate();

            if (simulate) {
                if (visualize) {
                    simulation.update_staggered(1.0 / 60);
                } else {
                    simulation.update(1. / 60.);
                }
            }

            updateSceneGraph();

            projViewUniform.view = camera->getViewMatrix();
            projViewUniform.syncView();

            pbrShaderNormalMapping->setUniform("camPos", camera->getPosition());
            pbrShader->setUniform("camPos", camera->getPosition());

            if (visualize) {
                baseRenderer->useShader(*csShader);
                baseRenderer->draw(*point);

                baseRenderer->enableWireframe();
                renderer->renderGraph(scene);
                baseRenderer->disableWireframe();

                simulation.visualize_collisions(projViewUniform.projection, projViewUniform.view);
            } else {
                renderer->renderGraph(scene);
            }

            window->swapBuffers();
            window->pollEvents();
        }
    }

public:
    bool simulate = false;
    bool visualize = false;

private:
    const double billiard_ball_radius = 0.03;
    const double billiard_ball_mass = 0.17;

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
    std::shared_ptr<gl3d::Mesh> barrier_mesh;

    physics3d::Material billiard_ball_material{
            .restitution = 0.99,
            .kinetic_friction = 0.2,
            .rolling_friction = 0.001,
    };

    physics3d::Material billiard_table_material{
            .restitution = 0.9,
            .kinetic_friction = 1.0,
            .rolling_friction = 1.0,
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
        scene_object.local_transform = gml::matrix::scale<double>(gml::Vec3d(billiard_ball_radius));
        scene_object.mesh = ball_mesh;

        auto rb = simulation.create_rigid_body(
                physics3d::InertiaShape::sphere(billiard_ball_radius, billiard_ball_mass),
                physics3d::colliders::Sphere{
                        .radius = billiard_ball_radius,
                },
                billiard_ball_material,
                position,
                gml::quaternion::fromMatrix(
                        gml::matrix::axisAngle(gml::Vec3d(0, 0, 1), std::numbers::pi_v<double> / 2).getRotation()));

        objects.emplace_back(scene_object, rb);
        return rb;
    }

    void load_balls(const gml::Vec3d& offset)
    {
        const double epsilon = 0.00000001;
        const double height =
                std::sqrt(-5 * 0.5 * 2 * billiard_ball_radius * 5 * 0.5 * 2 * billiard_ball_radius + 5 * 2 * billiard_ball_radius * 5 * 2 * billiard_ball_radius) / 5.;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < i + 1; ++j) {
                load_ball("models/old_billiard_ball.glb",
                          offset + gml::Vec3d(
                                  i * height + epsilon * i,
                                  0,
                                  -(i * billiard_ball_radius * 2) / 2.0 + j * billiard_ball_radius * 2 + j * epsilon));
            }
        }
    }

    std::shared_ptr<physics3d::RigidBody>
    load_barrier(const gml::Vec3d& position, const gml::Quatd& orientation, const gml::Vec3d& scale = gml::Vec3d(1))
    {
        if (!barrier_mesh) {
            barrier_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                             "models/barrier.glb", *glContext->getDrawableCreator(),
                                                             *glContext->getTextureCreator(), pbrShader,
                                                             pbrShaderNormalMapping).at(0);
        }

        auto rb_barrier = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::colliders::OrientedPlane{
                        .original_normal = {0, 0, -1},
                },
                billiard_table_material,
                position,
                orientation);

        auto& scene_barrier = scene->create_object("barrier");
        scene_barrier.mesh = barrier_mesh;
        scene_barrier.local_transform = gml::matrix::scale(scale);
        objects.emplace_back(scene_barrier, rb_barrier);

        return rb_barrier;
    }

    std::shared_ptr<physics3d::RigidBody>
    load_ground(const gml::Vec3d& position)
    {
        auto ground_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                             "models/ground.glb", *glContext->getDrawableCreator(),
                                                             *glContext->getTextureCreator(), pbrShader,
                                                             pbrShaderNormalMapping).at(0);

        auto rb_ground = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::colliders::OrientedPlane{
                        .original_normal = {0, -1, 0},
                },
                billiard_table_material,
                position,
                gml::Quatd());

        auto& scene_ground = scene->create_object("ground");
        scene_ground.mesh = ground_mesh;
        scene_ground.local_transform = gml::matrix::scale(1.0, 1.0, 0.5);

        objects.emplace_back(scene_ground, rb_ground);

        return rb_ground;
    }

    void setup_lights()
    {
        auto lightRes = std::make_shared<gl3d::PointLight>();
        lightRes->color = gml::Vec3f(30);

        auto& light = scene->create_object("light");
        light.light = lightRes;
        light.local_transform = gml::matrix::translate(gml::Vec3d(0, 3, 0));
    }
};
