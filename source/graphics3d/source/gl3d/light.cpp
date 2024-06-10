#include "light.h"

namespace yage::gl3d
{
    LightType Light::type()
    {
        return m_type;
    }

    Light::Light(LightType type)
            : m_type(type)
    {
    }

    void DirectionalLight::update_uniforms(gl::IShader& shader, std::size_t i)
    {
        shader.setUniform(std::string(ShaderSnippets::DIR_LIGHTS_NAME) + "[" + utils::toString(i) + "]." +
                          std::string(ShaderSnippets::DIR_LIGHT_DIRECTION_NAME),
                          direction);
        shader.setUniform(std::string(ShaderSnippets::DIR_LIGHTS_NAME) + "[" + utils::toString(i) + "]." +
                          std::string(ShaderSnippets::LIGHT_COLOR_NAME),
                          color);
    }

    void DirectionalLight::update_from_transform(math::Mat4d transform)
    {
        auto rotation = math::quaternion::from_matrix(transform.rotation());
        direction = static_cast<math::Vec3f>(rotation.forward_direction());
    }

    DirectionalLight::DirectionalLight() : Light(LightType::DIRECTIONAL_LIGHT)
    {
    }

    void PointLight::update_uniforms(gl::IShader& shader, std::size_t i)
    {
        shader.setUniform(std::string(ShaderSnippets::POINT_LIGHTS_NAME) + "[" + utils::toString(i) + "]." +
                          std::string(ShaderSnippets::POINT_LIGHT_POSITION_NAME),
                          position);
        shader.setUniform(std::string(ShaderSnippets::POINT_LIGHTS_NAME) + "[" + utils::toString(i) + "]." +
                          std::string(ShaderSnippets::LIGHT_COLOR_NAME),
                          color);
    }

    void PointLight::update_from_transform(math::Mat4d transform)
    {
        position = static_cast<math::Vec3f>(transform.translation());
    }

    PointLight::PointLight() : Light(LightType::POINT_LIGHT)
    {
    }
}
