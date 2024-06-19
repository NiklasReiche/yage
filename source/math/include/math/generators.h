#pragma once

#include "matrix.h"
#include "vector.h"
#include "quaternion.h"

namespace yage::math
{
    // forward declarations
    namespace quaternion
    {
        template<std::floating_point T>
        constexpr Quaternion<T> euler_angle(T yaw, T roll, T pitch);
    }

    namespace matrix
    {
        /**
         * Constructs a square diagonal matrix.
         *
         * @param init The values of the diagonal elements.
         */
        template<typename T, std::size_t M>
            requires StrictlyPositive<M>
        Matrix<T, M, M> diagonal(const std::array<T, M>& init)
        {
            Matrix<T, M, M> m;
            for (std::size_t i = 0; i < M; ++i) {
                m(i, i) = init.at(i);
            }
            return m;
        }

        /**
         * Constructs a translation matrix.
         *
         * @param x The x component.
         * @param y The y component.
         * @param z The z component.
         * @return The translation matrix.
        */
        template<typename T>
        Mat4<T> translate(const T& x, const T& y, const T& z)
        {
            Mat4<T> result = matrix::Id<T, 4>;
            result(0, 3) = x;
            result(1, 3) = y;
            result(2, 3) = z;
            return result;
        }

        /**
         * Constructs a translation matrix from a vector.
         *
         * @param translation The translation vector.
         * @return The translation matrix.
         */
        template<typename T>
        Mat4<T> translate(const Vec3<T>& translation)
        {
            Mat4<T> result = matrix::Id<T, 4>;
            result(0, 3) = translation.x();
            result(1, 3) = translation.y();
            result(2, 3) = translation.z();
            return result;
        }

        /**
         * Constructs a scaling matrix.
         *
         * @param x The x component.
         * @param y The y component.
         * @param z The z component.
         * @return The scaling matrix.
         */
        template<typename T>
        Mat4<T> scale(T x, T y, T z)
        {
            Mat4<T> result = matrix::Id<T, 4>;
            result(0, 0) = x;
            result(1, 1) = y;
            result(2, 2) = z;
            return result;
        }

        /**
         * Constructs a scaling matrix from a vector.
         *
         * @param value The scaling vector.
         * @return The scaling matrix.
         */
        template<typename T>
        Mat4<T> scale(const Vec3<T>& value)
        {
            Mat4<T> result = matrix::Id<T, 4>;
            result(0, 0) = value.x();
            result(1, 1) = value.y();
            result(2, 2) = value.z();
            return result;
        }

        /**
         * Constructs a rotation matrix from an axis-angle representation.
         *
         * @param axis The rotation axis.
         * @param angle The rotation angle around the rotation axis in radians.
         * @return The rotation matrix.
         */
        template<typename T>
        Mat4<T> axisAngle(const Vec3<T>& axis, const double angle)
        {
            Vec3<T> a = normalize(axis);
            const T& x = a.x();
            const T& y = a.y();
            const T& z = a.z();
            const T c = std::cos(angle);
            const T s = std::sin(angle);
            const T t = 1 - c;

            Mat4<T> result = matrix::Id<T, 4>;
            result(0, 0) = x * x * t + c;
            result(0, 1) = x * y * t - z * s;
            result(0, 2) = x * z * t + y * s;

            result(1, 0) = y * x * t + z * s;
            result(1, 1) = y * y * t + c;
            result(1, 2) = y * z * t - x * s;

            result(2, 0) = z * x * t - y * s;
            result(2, 1) = z * y * t + x * s;
            result(2, 2) = z * z * t + c;
            return result;
        }

        /**
         * Constructs a perspective projection matrix.
         *
         * @param fov The field of view in degrees.
         * @param aspect The aspect ratio.
         * @param near The near plane.
         * @param far The far plane.
         * @return the projection matrix.
         */
        template<std::floating_point T>
        Mat4<T> perspective(const T fov, const T aspect, const T near, const T far)
        {
            Mat4<T> result = matrix::Id<T, 4>;
            const T top = near * std::tan(to_rad(fov / 2));
            const T bottom = 0 - top;
            const T right = top * aspect;
            const T left = 0 - right;
            result(0, 0) = 2 * near / (right - left);
            result(0, 2) = (right + left) / (right - left);
            result(1, 1) = 2 * near / (top - bottom);
            result(1, 2) = (top + bottom) / (top - bottom);
            result(2, 2) = -((far + near) / (far - near));
            result(2, 3) = -(2 * far * near / (far - near));
            result(3, 2) = -1;
            result(3, 3) = 0;
            return result;
        }

        /**
         * Constructs an orthographic projection matrix.
         *
         * @param left The left plane.
         * @param right The right plane.
         * @param bottom The bottom plane.
         * @param top The top plane.
         * @param near The near plane.
         * @param far The far plane.
         * @return The projection matrix.
         */
        template<std::floating_point T>
        Mat4<T> orthographic(
            const T left, const T right,
            const T bottom, const T top,
            const T near, const T far)
        {
            Mat4<T> result = matrix::Id<T, 4>;
            result(0, 0) = 2 / (right - left);
            result(0, 3) = -((right + left) / (right - left));
            result(1, 1) = 2 / (top - bottom);
            result(1, 3) = -((top + bottom) / (top - bottom));
            result(2, 2) = -2 / (far - near);
            result(2, 3) = -((far + near) / (far - near));
            result(3, 3) = 1;
            return result;
        }

        /**
         * Constructs a Look-At view matrix.
         *
         * @param pos The camera position.
         * @param target The view target position.
         * @param up The camera up vector.
         * @return The view matrix.
         */
        template<std::floating_point T>
        Mat4<T> look_at(const Vec3<T>& pos, const Vec3<T>& target, const Vec3<T>& up)
        {
            Vec3<T> direction = normalize(pos - target);
            Vec3<T> right = normalize(cross(up, direction));
            Mat4<T> mat1(
                right.x(), right.y(), right.z(), 0.f,
                up.x(), up.y(), up.z(), 0.f,
                direction.x(), direction.y(), direction.z(), 0.f,
                0.f, 0.f, 0.f, 1.f
            );
            return mat1 * translate<T>(-pos.x(), -pos.y(), -pos.z());
        }

        /**
         * Constructs a Look-At view matrix where the camera's up vector aligns with the direction world's up vector, i.e.
         * the camera cannot roll around its view direction.
         *
         * @param pos The camera position.
         * @param target The view target position.
         * @return The view matrix.
         */
        template<std::floating_point T>
        Mat4<T> look_at(const Vec3<T>& pos, const Vec3<T>& target)
        {
            Vec3<T> reverse_direction = normalize(pos - target);
            Vec3<T> right = normalize(cross(vector::worldUp<T>(), reverse_direction));
            Vec3<T> up = normalize(cross(reverse_direction, right));
            Mat4<T> mat1(
                right.x(), right.y(), right.z(), 0.f,
                up.x(), up.y(), up.z(), 0.f,
                reverse_direction.x(), reverse_direction.y(), reverse_direction.z(), 0.f,
                0.f, 0.f, 0.f, 1.f
            );
            return mat1 * translate<T>(-pos.x(), -pos.y(), -pos.z());
        }

        /**
         * Constructs a rotation matrix from a quaternion.
         *
         * @return A matrix that represents the same rotation as the given quaternion.
         */
        template<typename T>
        constexpr Mat4<T> from_quaternion(const Quaternion<T>& quaternion)
        {
            const Quaternion<T> q = normalize(quaternion);

            Mat4<T> result = matrix::Id<T, 4>;
            result(0, 0) = 1 - 2 * q.y() * q.y() - 2 * q.z() * q.z();
            result(0, 1) = 2 * q.x() * q.y() - 2 * q.z() * q.w();
            result(0, 2) = 2 * q.x() * q.z() + 2 * q.y() * q.w();

            result(1, 0) = 2 * q.x() * q.y() + 2 * q.z() * q.w();
            result(1, 1) = 1 - 2 * q.x() * q.x() - 2 * q.z() * q.z();
            result(1, 2) = 2 * q.y() * q.z() - 2 * q.x() * q.w();

            result(2, 0) = 2 * q.x() * q.z() - 2 * q.y() * q.w();
            result(2, 1) = 2 * q.y() * q.z() + 2 * q.x() * q.w();
            result(2, 2) = 1 - 2 * q.x() * q.x() - 2 * q.y() * q.y();
            return result;
        }

        /**
         * Constructs a rotation matrix from an euler angle representation.
         *
         * @param yaw The rotation around the y axis in radians.
         * @param roll The rotation around the z axis in radians.
         * @param pitch The rotation around the x axis in radians.
         * @return A matrix representing the same rotation as the given euler angles.
         */
        template<std::floating_point T>
        [[nodiscard]]
        constexpr Mat4<T> euler_angle(const T yaw, const T roll, const T pitch)
        {
            return matrix::from_quaternion(quaternion::euler_angle(yaw, roll, pitch));
        }
    }

    namespace quaternion
    {
        /**
         * Constructs a quaternion from an axis-angle representation.
         *
         * @param axis The rotation axis.
         * @param angle The rotation angle around the rotation axis in radians.
         * @return A quaternion representing the same rotation as the given axis-angle.
         */
        template<std::floating_point T>
        [[nodiscard]]
        constexpr Quaternion<T> axis_angle(const Vec3<T>& axis, const T angle)
        {
            const Vec3<T> a = normalize(axis);
            const T s = std::sin(angle / 2);

            return Quaternion<T>(std::cos(angle / 2), a.x() * s, a.y() * s, a.z() * s);
        }

        /**
         * Constructs a quaternion from an euler angle representation.
         *
         * @param yaw The rotation around the y axis in radians.
         * @param roll The rotation around the z axis in radians.
         * @param pitch The rotation around the x axis in radians.
         * @return A quaternion representing the same rotation as the given euler angles.
         */
        template<std::floating_point T>
        [[nodiscard]]
        constexpr Quaternion<T> euler_angle(const T yaw, const T roll, const T pitch)
        {
            const T cy = std::cos(yaw / 2.);
            const T cr = std::cos(roll / 2.);
            const T cp = std::cos(pitch / 2.);
            const T sy = std::sin(yaw / 2.);
            const T sr = std::sin(roll / 2.);
            const T sp = std::sin(pitch / 2.);

            const T w = cy * cr * cp - sy * sr * sp;
            const T x = sy * sr * cp + cy * cr * sp;
            const T y = sy * cr * cp + cy * sr * sp;
            const T z = cy * sr * cp - sy * cr * sp;
            return Quaternion<T>(w, x, y, z);
        }

        /**
         * Constructs a quaternion from a given rotation matrix.
         *
         * @param matrix A rotation matrix.
         * @return A quaternion representing the same rotation as the given matrix.
         */
        template<std::floating_point T>
        [[nodiscard]]
        constexpr Quaternion<T> from_matrix(const Mat3<T>& matrix)
        {
            // see http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
            const T tr = trace(matrix);
            const Mat3<T>& m = matrix;
            Quaternion<T> q;
            if (tr > 0) {
                const T s = std::sqrt(tr + 1) * 2;
                q.w() = 0.25 * s;
                q.x() = (m(2, 1) - m(1, 2)) / s;
                q.y() = (m(0, 2) - m(2, 0)) / s;
                q.z() = (m(1, 0) - m(0, 1)) / s;
            } else if (m(0, 0) > m(1, 1) && m(0, 0) > m(2, 2)) {
                const T s = std::sqrt(1.0 + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
                q.w() = (m(2, 1) - m(1, 2)) / s;
                q.x() = 0.25 * s;
                q.y() = (m(0, 1) + m(1, 0)) / s;
                q.z() = (m(0, 2) + m(2, 0)) / s;
            } else if (m(1, 1) > m(2, 2)) {
                const T s = std::sqrt(1.0 + m(1, 1) - m(0, 0) - m(2, 2)) * 2;
                q.w() = (m(0, 2) - m(2, 0)) / s;
                q.x() = (m(0, 1) + m(1, 0)) / s;
                q.y() = 0.25 * s;
                q.z() = (m(1, 2) + m(2, 1)) / s;
            } else {
                const T s = std::sqrt(1.0 + m(2, 2) - m(0, 0) - m(1, 1)) * 2;
                q.w() = (m(1, 0) - m(0, 1)) / s;
                q.x() = (m(0, 2) + m(2, 0)) / s;
                q.y() = (m(1, 2) + m(2, 1)) / s;
                q.z() = 0.25 * s;
            }
            return q;
        }

        /**
         * Constructs a quaternion representing a Look-At view matrix where the camera's up vector aligns with the
         * world's up vector, i.e. the camera cannot roll around its view direction.
         *
         * @param position The camera position.
         * @param target The view target position.
         * @return The quaternion representation of the view matrix.
         */
        template<std::floating_point T>
        [[nodiscard]]
        constexpr Quaternion<T> look_at(const Vec3<T>& position, const Vec3<T>& target)
        {
            Mat4<T> m = matrix::look_at(position, target);
            return quaternion::from_matrix(m.rotation());
        }
    }
}
