#pragma once

#include <gml/vector.h>
#include <core/gl/Shader.h>
#include <utils/strings.h>

#include "shaderSnippets.h"

namespace gl3d
{
    class Light
    {
    public:
        gml::Vec3f color;

        virtual ~Light() = default;

        virtual void update_from_transform(gml::Mat4f transform) = 0;

        virtual void update_uniforms(gl::IShader& shader, std::size_t i) = 0;
    };

    class DirLight : public Light
    {
    public:
        gml::Vec3f direction;

        void update_uniforms(gl::IShader& shader, std::size_t i) override
        {
            shader.setUniform(ShaderSnippets::DIR_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                              ShaderSnippets::DIR_LIGHT_DIRECTION_NAME,
                              direction);
            shader.setUniform(ShaderSnippets::DIR_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                              ShaderSnippets::LIGHT_COLOR_NAME,
                              color);
        }

        void update_from_transform(gml::Mat4f transform) override
        {
            // TODO
        }
    };

    class PointLight : public Light
    {
    public:
        gml::Vec3f position;

        void update_uniforms(gl::IShader& shader, std::size_t i) override
        {
            shader.setUniform(ShaderSnippets::POINT_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                              ShaderSnippets::POINT_LIGHT_POSITION_NAME,
                              position);
            shader.setUniform(ShaderSnippets::POINT_LIGHTS_NAME + "[" + utils::toString(i) + "]." +
                              ShaderSnippets::LIGHT_COLOR_NAME,
                              color);
        }

        void update_from_transform(gml::Mat4f transform) override
        {
            position = transform.getTranslation();
        }
    };
}
