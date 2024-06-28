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
#include <gui/gui.h>

using namespace yage;

class App
{
public:
    explicit App(const std::string& filename)
    {
        window = std::make_shared<platform::desktop::GlfwWindow>(1500, 900, "Model Viewer");
        glContext = gl::createContext(window);

        camera = std::make_shared<gl3d::Camera>();
        camera->look_at(math::Vec3d(5, 5, 5), math::Vec3d(0, 0, 0));

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

        csShader->linkUniformBlock(renderer->projection_view().ubo());

        renderer->projection() = math::matrix::perspective<float>(90.0f, 1500.0f / 900.0f, 0.1f, 1000.0f);

        scene = loadScene(filename);

        const auto light = std::make_shared<gl3d::DirectionalLight>();
        light->color = math::Vec3f(5, 5, 5);
        auto& light_node = scene->create_object("light");
        light_node.local_transform = math::matrix::from_quaternion<double>(
                math::quaternion::euler_angle<double>(math::to_rad(180.0), math::to_rad(0.0), math::to_rad(45.0)));
        light_node.light = light;

        inputListener = MovementListener(window, camera);
        inputListener.pbrShader = pbrShader;
        inputListener.pbrShader2 = pbrShaderNormalMapping;
        inputListener.world = scene;
        window->attach(inputListener);

        font::FontFileLoader fontLoader(glContext->getTextureCreator(), window->getFileReader());
        font = fontStore.load_resource(fontLoader, std::string("assets/fonts/OpenSans-Regular.font"));
        gui = std::make_unique<gui::Master>(window, glContext);
        const auto frame = gui->create_widget<gui::ListBox>(gui::ListBoxTemplate{
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
                .text = U"Zoom in/out: scroll mouse",
                .font = font.value(),
            },
        });
        frame->create_widget<gui::Label>(gui::LabelTemplate{
            .text = {
                .text = U"Pan: left-click + drag mouse",
                .font = font.value(),
            },
        });
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

        renderer->active_camera = camera;
        renderer->active_scene = scene;

        while (!window->shouldDestroy()) {
            baseRenderer->clear();

            inputListener.applyUpdate();

            baseRenderer->useShader(*csShader);
            baseRenderer->draw(*point);

            baseRenderer->useShader(*pbrShader);

            renderer->render_active_scene();

            gui->update(1/60.);
            gui->render();

            window->swapBuffers();
            window->pollEvents();
        }
    }

private:
    std::shared_ptr<platform::IWindow> window;
    std::shared_ptr<gl::IContext> glContext;
    MovementListener inputListener;

    std::shared_ptr<gl::IShader> csShader;
    std::shared_ptr<gl::IShader> pbrShader;
    std::shared_ptr<gl::IShader> pbrShaderNormalMapping;
    std::shared_ptr<gl3d::Camera> camera;
    std::shared_ptr<gl::IRenderer> baseRenderer;
    std::unique_ptr<gl3d::SceneRenderer> renderer;
    res::Store<font::Font> fontStore;
    std::optional<res::Resource<font::Font>> font;
    std::unique_ptr<gui::Master> gui;

    std::shared_ptr<gl3d::SceneGroup> scene;

    std::shared_ptr<gl3d::SceneGroup> loadScene(const std::string& filename)
    {
        return gl3d::resources::gltf_read_scene(platform::desktop::FileReader(),
                                                filename, *glContext->getDrawableCreator(),
                                                *glContext->getTextureCreator(), renderer->shaders());
    }
};
