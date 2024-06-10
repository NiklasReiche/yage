#include "sceneRenderer.h"

namespace yage::gl3d
{
    struct Geometry
    {
        std::shared_ptr<Mesh> mesh;
        math::Mat4d transform;
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

        auto collectGeometry = [this, &drawablesLoop](SceneObject& node) {
            const auto& transform = node.world_transform();
            if (node.mesh) {
                drawablesLoop.emplace_back(node.mesh, transform);
            }
            if (node.light) {
                node.light->update_from_transform(transform);
                switch (node.light->type()) {
                    case LightType::DIRECTIONAL_LIGHT:
                        uniformValues.dir_lights.push_back(node.light);
                        break;
                    case LightType::POINT_LIGHT:
                        uniformValues.point_lights.push_back(node.light);
                        break;
                }
            }
            if (node.camera) {
                node.camera->moveTo(transform.translation());
                node.camera->rotateTo(math::quaternion::from_matrix<double>(transform.rotation()));
            }
        };

        // TODO: use found camera from scene graph to set shader camPos, like we do with lights

        root->apply(collectGeometry, math::matrix::Id4d);

        // TODO: sort by shader
        for (auto& geometry: drawablesLoop) {
            for (auto& sub_mesh: geometry.mesh->sub_meshes()) {
                auto shader = sub_mesh->material().shader();

                // TODO: lights should probably not be uniforms, but rather UBOs or SSBOs
                shader->setUniform(std::string(ShaderSnippets::DIR_LIGHTS_AMOUNT_NAME),
                                   static_cast<int>(uniformValues.dir_lights.size()));
                for (std::size_t i = 0; i < uniformValues.dir_lights.size(); ++i) {
                    uniformValues.dir_lights[i]->update_uniforms(*shader, i);
                }
                shader->setUniform(std::string(ShaderSnippets::POINT_LIGHTS_AMOUNT_NAME),
                                   static_cast<int>(uniformValues.point_lights.size()));
                for (std::size_t i = 0; i < uniformValues.point_lights.size(); ++i) {
                    uniformValues.point_lights[i]->update_uniforms(*shader, i);
                }

                shader->setUniform("model", static_cast<math::Mat4f>(geometry.transform));
                sub_mesh->material().update_shader_uniforms();
                sub_mesh->material().bind_textures(*renderer);

                renderer->draw(sub_mesh->drawable());
            }
        }
    }
}
