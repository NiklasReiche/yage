#include "sceneRenderer.h"

#include "shaderSnippets.h"

namespace yage::gl3d
{
    SceneRenderer::SceneRenderer(gl::IContext& context) :
        m_renderer(context.getRenderer()),
        m_projection_view(context.getShaderCreator()->createUniformBlock("ProjectionView"))
    {
        const std::shared_ptr<gl::IShaderCreator> shader_creator = context.getShaderCreator();
        m_shaders.emplace(ShaderPermutation::PBR,
                          shader_creator->createShader(shaders::PbrShader::get_vert(), shaders::PbrShader::get_frag()));
        m_shaders.emplace(ShaderPermutation::PBR_NORMAL_MAP,
                          shader_creator->createShader(shaders::PbrNormalMappingShader::get_vert(),
                                                       shaders::PbrNormalMappingShader::get_frag()));
    }

    void SceneRenderer::render_active_scene()
    {
        if (!active_scene)
            return;

        m_drawables.clear();
        m_uniform_values.point_lights.clear();
        m_uniform_values.dir_lights.clear();

        active_scene.value().get().apply([this](SceneObject& node) { collect_entities(node); }, math::matrix::Id4d);

        m_projection_view.view = static_cast<math::Mat4f>(active_camera->view_matrix());
        m_projection_view.sync();

        base_renderer().enableDepthTest();

        // TODO: sort by shader
        for (auto& node: m_drawables) {
            MeshResource& mesh = node.get().mesh.value();
            const math::Mat4d& transform = node.get().world_transform();

            for (const auto& sub_mesh: mesh.get().sub_meshes()) {
                auto shader = sub_mesh->material().shader();

                // TODO: don't do this every frame
                shader->linkUniformBlock(m_projection_view.ubo());

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

                if (shader->hasUniform("camPos")) {
                    shader->setUniform("camPos", static_cast<math::Vec3f>(active_camera->position()));
                }

                shader->setUniform("model", static_cast<math::Mat4f>(transform));
                sub_mesh->material().update_shader_uniforms();
                sub_mesh->material().bind_textures(*m_renderer);

                m_renderer->draw(sub_mesh->drawable());
            }
        }
    }

    math::Mat4f& SceneRenderer::projection()
    {
        return m_projection_view.projection;
    }

    math::Mat4f& SceneRenderer::view()
    {
        return m_projection_view.view;
    }

    const ProjectionView& SceneRenderer::projection_view()
    {
        return m_projection_view;
    }

    gl::IRenderer& SceneRenderer::base_renderer()
    {
        return *m_renderer;
    }

    ShaderMap SceneRenderer::shaders() const
    {
        return m_shaders;
    }

    void SceneRenderer::collect_entities(SceneObject& node)
    {
        const math::Mat4d& transform = node.world_transform();

        if (node.mesh) {
            m_drawables.emplace_back(node);
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
    }
}
