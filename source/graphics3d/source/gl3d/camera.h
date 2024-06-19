#pragma once

#include <math/math.h>

namespace yage::gl3d
{
    class Camera
    {
    public:
        Camera();

        Camera(const math::Vec3d& position, const math::Quatd& rotation);

        void move_by(const math::Vec3d& vector);

        void move_to(const math::Vec3d& position);

        void move_forward(double amount);

        void move_backward(double amount);

        void move_left(double amount);

        void move_right(double amount);

        void rotate_by(const math::Quatd& quaternion);

        void rotate_to(const math::Quatd& rotation);

        void rotate_yaw(double degree);

        void rotate_pitch(double degree);

        void rotate_roll(double degree);

        [[nodiscard]] math::Mat4d view_matrix() const;

        [[nodiscard]] math::Vec3d position() const;

        [[nodiscard]] math::Quatd rotation() const;

    private:
        math::Vec3d m_position;
        math::Quatd m_rotation;
    };
}
