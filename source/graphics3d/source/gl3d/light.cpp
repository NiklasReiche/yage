#include "light.h"
#include <utils/strings.h>
#include <math/generators.h>

namespace yage::gl3d
{
    void Light::update_uniforms(gl::IShader& shader, const std::size_t i)
    {
        for (LightModel light_model: light_models) {
            if (std::holds_alternative<PhongLightModel>(light_model)) {
                if (shader.hasUniform(uniform_prefix(i) + ".ambient")) {
                    shader.setUniform(uniform_prefix(i) + ".ambient", std::get<PhongLightModel>(light_model).ambient);
                    shader.setUniform(uniform_prefix(i) + ".diffuse", std::get<PhongLightModel>(light_model).diffuse);
                    shader.setUniform(uniform_prefix(i) + ".specular", std::get<PhongLightModel>(light_model).specular);
                }
            } else if (std::holds_alternative<PbrLightModel>(light_model)) {
                if (shader.hasUniform(uniform_prefix(i) + ".color"))
                    shader.setUniform(uniform_prefix(i) + ".color", std::get<PbrLightModel>(light_model).color);
            }
        }
    }

    LightType Light::type() const
    {
        return m_type;
    }

    Light::Light(const LightType type)
            : m_type(type)
    {
    }

    void DirectionalLight::update_uniforms(gl::IShader& shader, const std::size_t i)
    {
        Light::update_uniforms(shader, i);
        shader.setUniform(uniform_prefix(i) + ".direction", direction);
    }

    void DirectionalLight::update_from_transform(const math::Mat4d& transform)
    {
        const math::Quatd rotation = math::quaternion::from_matrix(transform.rotation());
        direction = static_cast<math::Vec3f>(rotation.forward_direction());
    }

    std::string DirectionalLight::uniform_prefix(const std::size_t i)
    {
        return "dirLights[" + std::to_string(i) + "]";
    }

    void DirectionalLight::update_global_uniforms(gl::IShader& shader, const std::size_t n_lights)
    {
        shader.setUniform("n_dirLights", static_cast<int>(n_lights));
    }

    DirectionalLight::DirectionalLight() : Light(LightType::DIRECTIONAL_LIGHT)
    {
    }

    void PointLight::update_uniforms(gl::IShader& shader, const std::size_t i)
    {
        Light::update_uniforms(shader, i);
        shader.setUniform(uniform_prefix(i) + ".position", position);
    }

    void PointLight::update_from_transform(const math::Mat4d& transform)
    {
        position = static_cast<math::Vec3f>(transform.translation());
    }

    std::string PointLight::uniform_prefix(const std::size_t i)
    {
        return "pointLights[" + std::to_string(i) + "]";
    }

    void PointLight::update_global_uniforms(gl::IShader& shader, const std::size_t n_lights)
    {
        shader.setUniform("n_pointLights", static_cast<int>(n_lights));
    }

    PointLight::PointLight() : Light(LightType::POINT_LIGHT)
    {
    }
}
