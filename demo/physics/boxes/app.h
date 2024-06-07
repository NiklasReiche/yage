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
#include <chrono>

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
                gl3d::Camera(gml::Vec3f(25.0f, 25.0f, 25.0f),
                             gml::quaternion::eulerAngle<double>(gml::toRad(225.), 0, 0) *
                             gml::quaternion::eulerAngle<double>(0, 0, gml::toRad(30.))));

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

    void add_box()
    {
        double height = box_stack * (box_length + 0.5);
        load_cube("models/box.glb", box_offset + gml::Vec3d(0, height, 0));
        box_stack++;
    }

    void throw_box()
    {
        auto rb = load_cube("models/box.glb", box_offset + gml::Vec3d(-20, 10, 0));
        rb->apply_force(gml::Vec3d(800, 1000, 0), rb->position() + gml::Vec3d(0.2, 0, 0.1));
    }

    void add_box_rotated()
    {
        double height = box_stack * (box_length + 0.5);
        load_cube("models/box.glb", box_offset + gml::Vec3d(0, height, 0),
                  gml::quaternion::eulerAngle<double>(0.0, gml::toRad(10.0), gml::toRad(30.0)));
        box_stack++;
    }

    void run()
    {
        window->show();
        std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();

        auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{},
                                                                     std::vector<unsigned int>{},
                                                                     {},
                                                                     gl::VertexFormat::INTERLEAVED);

        load_ground();

        constexpr const double dt = 1. / 60.;
        window->getTimeStep();
        double accumulator = 0.0;
        while (!window->shouldDestroy()) {
            double frame_time = window->getTimeStep();

            baseRenderer->clear();

            inputListener.applyUpdate();

            if (simulate) {
                accumulator += frame_time;
                while (accumulator >= dt) {
                    if (visualize) {
                        simulation.update_staggered(dt);
                    } else {
                        simulation.update(dt);
                    }
                    accumulator -= dt;
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
    int box_stack = 0;
    int box_mass = 1;
    const gml::Vec3d box_offset = gml::Vec3d(0, 1.2, 0);
    const int box_length = 2;

    std::shared_ptr<platform::desktop::GlfwWindow> window;
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

    std::shared_ptr<gl3d::Mesh> cube_mesh;

    physics3d::Material ground_material{
            .restitution = 0.0,
            .kinetic_friction = 1.0,
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
    load_cube(const std::string& filename, gml::Vec3d position, gml::Quatd orientation = gml::Quatd())
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
                physics3d::InertiaShape::cube(2, box_mass),
                physics3d::colliders::OrientedBox{
                        .half_size = gml::Vec3d(1, 1, 1),
                },
                cube_material,
                position,
                orientation);

        objects.emplace_back(scene_object, rb);
        return rb;
    }

    void load_ground()
    {
        auto mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                      "models/ground.glb", *glContext->getDrawableCreator(),
                                                      *glContext->getTextureCreator(), pbrShader,
                                                      pbrShaderNormalMapping).at(0);

        auto& scene_object = scene->create_object("ground");
        scene_object.mesh = mesh;

        auto rb = simulation.create_rigid_body(
                physics3d::InertiaShape::static_shape(),
                physics3d::colliders::OrientedPlane{
                        .original_normal = {0, -1, 0},
                },
                ground_material,
                gml::Vec3d(0),
                gml::Quatd());

        objects.emplace_back(scene_object, rb);
    }

    void setup_lights()
    {
        auto lightRes = std::make_shared<gl3d::PointLight>();
        lightRes->color = gml::Vec3f(100);

        auto& light = scene->create_object("light");
        light.light = lightRes;
        light.local_transform =
                gml::matrix::translate(-10, 10, -10);

        auto lightRes2 = std::make_shared<gl3d::DirectionalLight>();
        lightRes2->color = gml::Vec3f(3);

        auto& light2 = scene->create_object("light");
        light2.light = lightRes2;
        light2.local_transform =
                gml::matrix::fromQuaternion<double>(
                        gml::quaternion::eulerAngle<double>(gml::toRad(200.), 0, 0) *
                        gml::quaternion::eulerAngle<double>(0, 0, gml::toRad(60.))
                );

    }
};
