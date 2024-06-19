#include "sceneRenderer.h"
#include "shaderSnippets.h"

namespace yage::gl3d
{
    struct Geometry
    {
        std::shared_ptr<Mesh> mesh;
        math::Mat4d transform;
    };

    SceneRenderer::SceneRenderer(std::shared_ptr<gl::IRenderer> renderer)
            : m_renderer(std::move(renderer))
    {
    }

    void SceneRenderer::render_graph(const std::shared_ptr<SceneNode>& root)
    {
        std::vector<Geometry> drawablesLoop;
        m_uniform_values.point_lights.clear();
        m_uniform_values.dir_lights.clear();

        auto collectGeometry = [this, &drawablesLoop](SceneObject& node) {
            const auto& transform = node.world_transform();
            if (node.mesh) {
                drawablesLoop.emplace_back(node.mesh, transform);
            }
            if (node.light) {
                node.light->update_from_transform(transform);
                switch (node.light->type()) {
                    case LightType::DIRECTIONAL_LIGHT:
                        m_uniform_values.dir_lights.push_back(node.light);
                        break;
                    case LightType::POINT_LIGHT:
                        m_uniform_values.point_lights.push_back(node.light);
                        break;
                }
            }
            if (node.camera) {
                node.camera->move_to(transform.translation());
                node.camera->rotate_to(math::quaternion::from_matrix<double>(transform.rotation()));
            }
        };

        // TODO: use found camera from scene graph to set shader camPos, like we do with lights

        root->apply(collectGeometry, math::matrix::Id4d);

        // TODO: sort by shader
        for (auto& [mesh, transform]: drawablesLoop) {
            for (const auto& sub_mesh: mesh->sub_meshes()) {
                auto shader = sub_mesh->material().shader();

                // TODO: lights should probably not be uniforms, but rather UBOs or SSBOs
                shader->setUniform(std::string(ShaderSnippets::DIR_LIGHTS_AMOUNT_NAME),
                                   static_cast<int>(m_uniform_values.dir_lights.size()));
                for (std::size_t i = 0; i < m_uniform_values.dir_lights.size(); ++i) {
                    m_uniform_values.dir_lights[i]->update_uniforms(*shader, i);
                }
                shader->setUniform(std::string(ShaderSnippets::POINT_LIGHTS_AMOUNT_NAME),
                                   static_cast<int>(m_uniform_values.point_lights.size()));
                for (std::size_t i = 0; i < m_uniform_values.point_lights.size(); ++i) {
                    m_uniform_values.point_lights[i]->update_uniforms(*shader, i);
                }

                shader->setUniform("model", static_cast<math::Mat4f>(transform));
                sub_mesh->material().update_shader_uniforms();
                sub_mesh->material().bind_textures(*m_renderer);

                m_renderer->draw(sub_mesh->drawable());
            }
        }
    }
}
