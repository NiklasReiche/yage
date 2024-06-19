#pragma once

#include <math/vector.h>
#include <core/gl/Shader.h>

namespace yage::gl3d
{
    enum class LightType
    {
        DIRECTIONAL_LIGHT,
        POINT_LIGHT
    };

    class Light
    {
    public:
        math::Vec3f color;

        virtual ~Light() = default;

        virtual void update_from_transform(const math::Mat4d& transform) = 0;

        virtual void update_uniforms(gl::IShader& shader, std::size_t i) = 0;

        [[nodiscard]] LightType type() const;

    protected:
        explicit Light(LightType type);

    private:
        LightType m_type;
    };

    class DirectionalLight final : public Light
    {
    public:
        math::Vec3f direction;

        DirectionalLight();

        void update_uniforms(gl::IShader& shader, std::size_t i) override;

        void update_from_transform(const math::Mat4d& transform) override;
    };

    class PointLight final : public Light
    {
    public:
        math::Vec3f position;

        PointLight();

        void update_uniforms(gl::IShader& shader, std::size_t i) override;

        void update_from_transform(const math::Mat4d& transform) override;
    };
}
