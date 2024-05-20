#pragma once

#include <gml/vector.h>
#include <core/gl/Shader.h>
#include <utils/strings.h>

#include "shaderSnippets.h"

namespace gl3d
{
    enum class LightType
    {
        DIRECTIONAL_LIGHT,
        POINT_LIGHT
    };

    class Light
    {
    public:
        gml::Vec3f color;

        virtual ~Light() = default;

        virtual void update_from_transform(gml::Mat4d transform) = 0;

        virtual void update_uniforms(gl::IShader& shader, std::size_t i) = 0;

        LightType type();

    protected:
        explicit Light(LightType type);

    private:
        LightType m_type;
    };

    class DirectionalLight : public Light
    {
    public:
        gml::Vec3f direction;

        DirectionalLight();

        void update_uniforms(gl::IShader& shader, std::size_t i) override;

        void update_from_transform(gml::Mat4d transform) override;
    };

    class PointLight : public Light
    {
    public:
        gml::Vec3f position;

        PointLight();

        void update_uniforms(gl::IShader& shader, std::size_t i) override;

        void update_from_transform(gml::Mat4d transform) override;
    };
}
