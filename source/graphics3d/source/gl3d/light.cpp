#include "light.h"

namespace gl3d
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
        shader.setUniform(ShaderSnippets::DIR_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                          ShaderSnippets::DIR_LIGHT_DIRECTION_NAME,
                          direction);
        shader.setUniform(ShaderSnippets::DIR_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                          ShaderSnippets::LIGHT_COLOR_NAME,
                          color);
    }

    void DirectionalLight::update_from_transform(gml::Mat4d transform)
    {
        auto rotation = gml::quaternion::fromMatrix(transform.getRotation());
        direction = rotation.getForward();
    }

    DirectionalLight::DirectionalLight() : Light(LightType::DIRECTIONAL_LIGHT)
    {
    }

    void PointLight::update_uniforms(gl::IShader& shader, std::size_t i)
    {
        shader.setUniform(ShaderSnippets::POINT_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                          ShaderSnippets::POINT_LIGHT_POSITION_NAME,
                          position);
        shader.setUniform(ShaderSnippets::POINT_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                          ShaderSnippets::LIGHT_COLOR_NAME,
                          color);
    }

    void PointLight::update_from_transform(gml::Mat4d transform)
    {
        position = transform.getTranslation();
    }

    PointLight::PointLight() : Light(LightType::POINT_LIGHT)
    {
    }
}
