#pragma once

#include "core/platform/Window.h"
#include "core/platform/desktop/GlfwWindow.h"
#include "core/platform/IFileReader.h"
#include "core/platform/desktop/FileReader.h"
#include "core/gl/Context.h"
#include "math/math.h"
#include "gl3d/camera.h"
#include "gl3d/sceneRenderer.h"
#include "gl3d/resources/obj.h"
#include "gl3d/resources/gltf.h"
#include "gl3d/light.h"
#include "physics3d/Simulation.h"

#include "MovementListener.h"
#include "gl3d/shaders.h"
#include "gui/gui.h"

using namespace yage;

class App
{
public:
    bool simulate = true;
    bool visualize = false;

    App()
    {
        window = std::make_shared<platform::desktop::GlfwWindow>(1500, 900, "Sandbox");
        glContext = gl::createContext(window);

        simulation = physics3d::Simulation(physics3d::Visualizer(*glContext));
        simulation.enable_gravity();

        camera = std::make_shared<gl3d::Camera>();
        camera->look_at(math::Vec3d(0.0, 2.0, 2.0), math::Vec3d(0.0));

        renderer = std::make_shared<gl3d::SceneRenderer>(*glContext);
        renderer->base_renderer().setClearColor(0x008080FFu);
        renderer->base_renderer().enableDepthTest();
        renderer->base_renderer().setViewport(0, 0, 1500, 900);

        const auto fileReader = platform::desktop::FileReader();
        const std::string csVertexShader = fileReader.openTextFile(
            "shaders/pointShader.vert", platform::IFile::AccessMode::READ)->readAll();
        const std::string csFragmentShader = fileReader.openTextFile(
            "shaders/pointShader.frag", platform::IFile::AccessMode::READ)->readAll();
        const std::string csGeometryShader = fileReader.openTextFile(
            "shaders/pointShader.geom", platform::IFile::AccessMode::READ)->readAll();
        csShader = glContext->getShaderCreator()->createShader(csVertexShader, csFragmentShader, csGeometryShader);

        pbrShaderNormalMapping = glContext->getShaderCreator()->createShader(
            gl3d::shaders::PbrNormalMappingShader::vert, gl3d::shaders::PbrNormalMappingShader::frag);
        pbrShader = glContext->getShaderCreator()->createShader(
            gl3d::shaders::PbrShader::vert, gl3d::shaders::PbrShader::frag);

        auto ubo = glContext->getShaderCreator()->createUniformBlock("ProjectionView");
        csShader->linkUniformBlock(renderer->projection_view().ubo());

        renderer->projection() = math::matrix::perspective<float>(45.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);

        scene = std::make_shared<gl3d::SceneGroup>("world");
        setup_lights();

        inputListener = MovementListener(window, camera, &simulation, this);
        window->attach(inputListener);

        font::FontFileLoader fontLoader(glContext->getTextureCreator(), window->getFileReader());
        font = fontStore.loadResource(fontLoader, std::string("fonts/OpenSans-Regular.font"));
        gui = std::make_unique<gui::Master>(window, glContext);
        auto frame = gui->create_widget<gui::ListBox>(gui::ListBoxTemplate{
            .base = {
                .geometry = {
                    .size_hint = math::Vec2(gui::SizeHint::FIT_CHILDREN),
                },
                .color = gl::Color::WHITE,
                .padding = {2, 2, 2, 2},
            },
            .orientation = gui::ListBoxTemplate::VERTICAL,
        });
        frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text = {
                .text = U"Controls:",
                .font = font.value(),
            },
        });
        frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text = {
                .text = U"x: toggle mouse capture for looking around",
                .font = font.value(),
            },
        });
        frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text = {
                .text = U"mouse: look around",
                .font = font.value(),
            },
        });
        frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text = {
                .text = U"w,a,s,d: move around",
                .font = font.value(),
            },
        });
        frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text = {
                .text = U"enter: shoot ball",
                .font = font.value(),
            },
        });
    }


    void run()
    {
        window->show();
        std::static_pointer_cast<platform::desktop::GlfwWindow>(window)->getTimeStep();


        const auto ground = load_ground(math::Vec3d(0));

        load_barrier(math::Vec3d(1, 0, 0),
                     math::quaternion::euler_angle<double>(-std::numbers::pi_v<double> / 2, 0, 0),
                     math::Vec3d(0.5, 1.0, 1.0));

        load_barrier(math::Vec3d(0, 0, 0.5),
                     math::quaternion::euler_angle<double>(std::numbers::pi_v<double>, 0, 0));

        load_barrier(math::Vec3d(0, 0, -0.5), math::Quatd());

        load_barrier(math::Vec3d(-1, 0, 0),
                     math::quaternion::euler_angle<double>(std::numbers::pi_v<double> / 2, 0, 0),
                     math::Vec3d(0.5, 1.0, 1.0));

        load_balls(math::Vec3d(0.35, ground->position().y() + billiard_ball_radius + 0.00001, 0));

        const auto ball = load_ball("models/old_billiard_ball.glb",
                                    math::Vec3d(-0.5, ground->position().y() + billiard_ball_radius + 0.00001, 0));
        inputListener.ball = ball.get();


        const auto point = glContext->getDrawableCreator()->createDrawable(std::vector<float>{},
                                                                           std::vector<unsigned int>{},
                                                                           {},
                                                                           gl::VertexFormat::INTERLEAVED);

        constexpr double dt = 1. / 60.;
        window->getTimeStep();
        double accumulator = 0.0;
        while (!window->shouldDestroy()) {
            const double frame_time = window->getTimeStep();

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

            renderer->base_renderer().clear();

            updateSceneGraph();

            renderer->view() = static_cast<math::Mat4f>(camera->view_matrix());

            if (visualize) {
                renderer->base_renderer().useShader(*csShader);
                renderer->base_renderer().draw(*point);

                renderer->base_renderer().enableWireframe();
                renderer->render_graph(scene, *camera);
                renderer->base_renderer().disableWireframe();

                simulation.visualize_collisions(static_cast<math::Mat4d>(renderer->projection()),
                                                static_cast<math::Mat4d>(renderer->view()));
            } else {
                renderer->render_graph(scene, *camera);
            }

            gui->update(1 / 60.);
            gui->render();

            window->swapBuffers();
            window->pollEvents();
        }
    }

private:
    const double billiard_ball_radius = 0.03;
    const double billiard_ball_mass = 0.17;

    std::shared_ptr<platform::desktop::GlfwWindow> window;
    std::shared_ptr<gl::IContext> glContext;
    MovementListener inputListener;

    std::shared_ptr<gl::IShader> shader;
    std::shared_ptr<gl::IShader> csShader;
    std::shared_ptr<gl::IShader> pbrShaderNormalMapping;
    std::shared_ptr<gl::IShader> pbrShader;
    std::shared_ptr<gl3d::Camera> camera;
    std::shared_ptr<gl3d::SceneRenderer> renderer;
    res::Store<font::Font> fontStore;
    std::optional<res::Resource<font::Font> > font;
    std::unique_ptr<gui::Master> gui;

    std::shared_ptr<gl3d::SceneGroup> scene;

    physics3d::Simulation simulation;
    std::vector<std::tuple<std::reference_wrapper<gl3d::SceneNode>, std::shared_ptr<physics3d::RigidBody> > > objects;

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
            object.get().local_transform = math::matrix::translate(rb->position()) *
                                           math::matrix::from_quaternion(rb->orientation()) *
                                           math::matrix::scale(object.get().local_transform.scale());
        }
    }

    std::shared_ptr<physics3d::RigidBody>
    load_ball(const std::string& filename, math::Vec3d position)
    {
        if (!ball_mesh) {
            ball_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
                                                          filename, *glContext->getDrawableCreator(),
                                                          *glContext->getTextureCreator(), pbrShader,
                                                          pbrShaderNormalMapping).at(0);
        }

        auto& scene_object = scene->create_object("ball");
        scene_object.local_transform = math::matrix::scale<double>(math::Vec3d(billiard_ball_radius));
        scene_object.mesh = ball_mesh;

        auto rb = simulation.create_rigid_body(
            physics3d::InertiaShape::sphere(billiard_ball_radius, billiard_ball_mass),
            physics3d::colliders::Sphere{
                .radius = billiard_ball_radius,
            },
            billiard_ball_material,
            position,
            math::quaternion::from_matrix(
                math::matrix::axisAngle(math::Vec3d(0, 0, 1), std::numbers::pi_v<double> / 2).rotation()));

        objects.emplace_back(scene_object, rb);
        return rb;
    }

    void load_balls(const math::Vec3d& offset)
    {
        const double height =
                std::sqrt(
                    -5 * 0.5 * 2 * billiard_ball_radius * 5 * 0.5 * 2 * billiard_ball_radius + 5 * 2 *
                    billiard_ball_radius * 5 * 2 * billiard_ball_radius) / 5.;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < i + 1; ++j) {
                constexpr double epsilon = 0.00000001;
                load_ball("models/old_billiard_ball.glb",
                          offset + math::Vec3d(
                              i * height + epsilon * i,
                              0,
                              -(i * billiard_ball_radius * 2) / 2.0 + j * billiard_ball_radius * 2 + j * epsilon));
            }
        }
    }

    std::shared_ptr<physics3d::RigidBody>
    load_barrier(const math::Vec3d& position, const math::Quatd& orientation, const math::Vec3d& scale = math::Vec3d(1))
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
        scene_barrier.local_transform = math::matrix::scale(scale);
        objects.emplace_back(scene_barrier, rb_barrier);

        return rb_barrier;
    }

    std::shared_ptr<physics3d::RigidBody>
    load_ground(const math::Vec3d& position)
    {
        const auto ground_mesh = gl3d::resources::gltf_read_meshes(platform::desktop::FileReader(),
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
            math::Quatd());

        auto& scene_ground = scene->create_object("ground");
        scene_ground.mesh = ground_mesh;
        scene_ground.local_transform = math::matrix::scale(1.0, 1.0, 0.5);

        objects.emplace_back(scene_ground, rb_ground);

        return rb_ground;
    }

    void setup_lights()
    {
        const auto lightRes = std::make_shared<gl3d::PointLight>();
        lightRes->color = math::Vec3f(30);

        auto& light = scene->create_object("light");
        light.light = lightRes;
        light.local_transform = math::matrix::translate(math::Vec3d(0, 3, 0));
    }
};
