#include "sceneRenderer.h"

namespace gl3d
{
    struct Geometry
    {
        std::shared_ptr<Mesh> mesh;
        gml::Mat4d transform;
    };

    SceneRenderer::SceneRenderer(std::shared_ptr<gl::IRenderer> renderer)
            : renderer(std::move(renderer))
    {
    }

    void SceneRenderer::renderGraph(const std::shared_ptr<SceneNode>& root)
    {
        std::vector<Geometry> drawablesLoop;
        uniformValues.point_lights.clear();
        uniformValues.dir_lights.clear();

        auto collectGeometry = [this, &drawablesLoop](SceneObject* node, gml::Mat4d transform) {
            if (node->hasMesh()) {
                drawablesLoop.emplace_back(node->getMesh(), transform);
            }
            if (node->hasLight()) {
                std::shared_ptr<Light> light = node->getLight();
                light->update_from_transform(transform);
                switch (light->type()) {
                    case LightType::DIRECTIONAL_LIGHT:
                        uniformValues.dir_lights.push_back(light);
                        break;
                    case LightType::POINT_LIGHT:
                        uniformValues.point_lights.push_back(light);
                        break;
                }
            }
            if (node->hasCamera()) {
                std::shared_ptr<Camera> camera = node->getCamera();
                camera->moveTo(transform.getTranslation());
                camera->rotateTo(gml::quaternion::fromMatrix<double>(transform.getRotation()));
            }
        };

        // TODO: use found camera from scene graph to set shader camPos, like we do with lights

        root->updateChildren(collectGeometry);

        // TODO: sort by shader
        for (auto& geometry: drawablesLoop) {
            for (auto& sub_mesh: geometry.mesh->sub_meshes()) {
                auto shader = sub_mesh->material().shader();

                // TODO: lights should probably not be uniforms, but rather UBOs or SSBOs
                shader->setUniform(gl3d::ShaderSnippets::DIR_LIGHTS_AMOUNT_NAME,
                                   static_cast<int>(uniformValues.dir_lights.size()));
                for (std::size_t i = 0; i < uniformValues.dir_lights.size(); ++i) {
                    uniformValues.dir_lights[i]->update_uniforms(*shader, i);
                }
                shader->setUniform(gl3d::ShaderSnippets::POINT_LIGHTS_AMOUNT_NAME,
                                   static_cast<int>(uniformValues.point_lights.size()));
                for (std::size_t i = 0; i < uniformValues.point_lights.size(); ++i) {
                    uniformValues.point_lights[i]->update_uniforms(*shader, i);
                }

                shader->setUniform("model", geometry.transform);
                sub_mesh->material().update_shader_uniforms();
                sub_mesh->material().bind_textures(*renderer);

                renderer->draw(sub_mesh->drawable());
            }
        }
    }
}
