#pragma once

#include <ostream>
#include <numbers>

#include "vector.h"

namespace gml
{
    template<typename T, std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
    class Matrix;

    template<typename T>
    using Mat3 = Matrix<T, 3, 3>;

    template<typename T>
    class Quaternion;

    template<typename T>
    using Quat = Quaternion<T>;

    using Quati = Quaternion<int>;
    using Quatf = Quaternion<float>;
    using Quatd = Quaternion<double>;

    template<typename T>
    constexpr T length_sqr(const Quaternion<T>& q);

    template<std::floating_point T>
    constexpr T length(const Quaternion<T>& q);

    template<std::floating_point T>
    constexpr Quaternion<T> normalize(Quaternion<T> q);

    template<typename T>
    constexpr Quaternion<T> conjugate(Quaternion<T> q);


    /**
     * Represents a generic quaternion.
     *
     * @tparam T The type of the quaternion's components.
     */
    template<typename T>
    class Quaternion
    {
    public:
        /**
         * Initializes an identity quaternion.
         */
        constexpr Quaternion()
            : m_w(1), m_x(0), m_y(0), m_z(0)
        {
        }

        /**
         * Initializes the given values.
         *
         * @param w The quaternion's w component.
         * @param x The quaternion's x component.
         * @param y The quaternion's y component.
         * @param z The quaternion's z component.
         */
        constexpr Quaternion(T w, T x, T y, T z)
            : m_w(w), m_x(x), m_y(y), m_z(z)
        {
        }

        /**
         * Constructs a pure quaternion from a vector.
         *
         * @param v The vector to construct from.
         */
        constexpr explicit Quaternion(const Vec3<T>& v)
            : m_w(0), m_x(v.x()), m_y(v.y()), m_z(v.z())
        {
        }

        [[nodiscard]]
        constexpr T& w()
        {
            return m_w;
        }

        [[nodiscard]]
        constexpr const T& w() const
        {
            return m_w;
        }

        [[nodiscard]]
        constexpr T& x()
        {
            return m_x;
        }

        [[nodiscard]]
        constexpr const T& x() const
        {
            return m_x;
        }

        [[nodiscard]]
        constexpr T& y()
        {
            return m_y;
        }

        [[nodiscard]]
        constexpr const T& y() const
        {
            return m_y;
        }

        [[nodiscard]]
        constexpr T& z()
        {
            return m_z;
        }

        [[nodiscard]]
        constexpr const T& z() const
        {
            return m_z;
        }

        /**
         * Normalizes this quaternion in place.
         *
         * @return A reference to this quaternion.
         */
        constexpr Quaternion& normalize() requires std::floating_point<T>
        {
            const T len = length(*this);
            assert(len != 0);
            return operator*=(1 / len);
        }

        /**
         * Conjugates this quaternion in-place.
         *
         * @return A reference to this quaternion.
         */
        constexpr Quaternion& conjugate()
        {
            m_x *= -1;
            m_y *= -1;
            m_z *= -1;
            return *this;
        }

        /**
         * Extracts the Forward vector of the rotation expressed by this quaternion.
         *
         * @return the Forward vector
         */
        [[nodiscard]]
        constexpr Vec3<T> forward_direction() const
        {
            Quaternion q = gml::normalize(*this);
            Quaternion result = q * Quaternion(vector::worldForward<T>()) * gml::conjugate(q);
            return gml::normalize(Vec3<T>(result.m_x, result.m_y, result.m_z));
        }

        /**
         * Extracts the Right vector of the rotation expressed by this quaternion.
         *
         * @return the Right vector
         */
        [[nodiscard]]
        constexpr Vec3<T> right_direction() const
        {
            Quaternion q = gml::normalize(*this);
            Quaternion result = q * Quaternion(vector::worldRight<T>()) * gml::conjugate(q);
            return gml::normalize(Vec3<T>(result.m_x, result.m_y, result.m_z));
        }

        /**
         * Extracts the Up vector of the rotation expressed by this quaternion.
         *
         * @return the Up vector
         */
        [[nodiscard]]
        constexpr Vec3<T> up_direction() const
        {
            Quaternion q = gml::normalize(*this);
            Quaternion result = q * Quaternion(vector::worldUp<T>()) * gml::conjugate(q);
            return gml::normalize(Vec3<T>(result.m_x, result.m_y, result.m_z));
        }

        /**
         * @return The pitch component of this quaternion's rotation in radians.
         */
        [[nodiscard]]
        constexpr T pitch() const requires std::floating_point<T>
        {
            const T unit = length_sqr(*this);
            if (const double test = m_x * m_y + m_z * m_w;
                test > 0.499 * unit || test < -0.499 * unit) {
                return 0; // singularity at north and south pole
            }
            const T sqw = m_w * m_w;
            const T sqx = m_x * m_x;
            const T sqy = m_y * m_y;
            const T sqz = m_z * m_z;
            return std::atan2(2 * m_x * m_w - 2 * m_y * m_z, -sqx + sqy - sqz + sqw);
        }


        /**
         * @return The yaw component of this quaternion's rotation in radians.
         */
        [[nodiscard]]
        constexpr T yaw() const requires std::floating_point<T>
        {
            const T unit = length_sqr(*this);
            const T test = m_x * m_y + m_z * m_w;
            if (test > 0.499 * unit) {
                return 2 * std::atan2(m_x, m_w); // singularity at north pole
            }
            if (test < -0.499 * unit) {
                return -2 * std::atan2(m_x, m_w); // singularity at south pole
            }
            const T sqw = m_w * m_w;
            const T sqx = m_x * m_x;
            const T sqy = m_y * m_y;
            const T sqz = m_z * m_z;
            return std::atan2(2 * m_y * m_w - 2 * m_x * m_z, sqx - sqy - sqz + sqw);
        }

        /**
         * @return The roll component of this quaternion's rotation in radians.
         */
        [[nodiscard]]
        constexpr T roll() const requires std::floating_point<T>
        {
            const T unit = length_sqr(*this);
            const T test = m_x * m_y + m_z * m_w;
            if (test > 0.499 * unit) {
                return std::numbers::pi / 2; // singularity at north pole
            }
            if (test < -0.499 * unit) {
                return -std::numbers::pi / 2; // singularity at south pole
            }
            return std::asin(2 * test / unit);
        }

        constexpr Quaternion& operator*=(T rhs)
        {
            m_x *= rhs;
            m_y *= rhs;
            m_z *= rhs;
            m_w *= rhs;
            return *this;
        }

        constexpr Quaternion& operator*=(const Quaternion& rhs)
        {
            const Vec3<T> a = Vec3<T>(m_x, m_y, m_z);
            const Vec3<T> b = Vec3<T>(rhs.m_x, rhs.m_y, rhs.m_z);

            const Vec3<T> c = m_w * b + rhs.m_w * a + cross(a, b);

            m_w = m_w * rhs.m_w - dot(a, b);
            m_x = c.x();
            m_y = c.y();
            m_z = c.z();
            return *this;
        }

        constexpr Quaternion& operator+=(const Quaternion& rhs)
        {
            m_w += rhs.m_w;
            m_x += rhs.m_x;
            m_y += rhs.m_y;
            m_z += rhs.m_z;
            return *this;
        }

    private:
        T m_w, m_x, m_y, m_z;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const Quaternion<T>& rhs)
    {
        os << "[" << rhs.w() << " " << rhs.x() << " " << rhs.y() << " " << rhs.z() << "]";
        return os;
    }

    template<typename T>
    constexpr bool operator==(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    {
        return lhs.w() == rhs.w() && lhs.x() == rhs.x() && lhs.y() == rhs.y() && lhs.z() == rhs.z();
    }

    template<typename T>
    constexpr bool operator!=(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template<typename T>
    constexpr Quaternion<T> operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
    {
        return Quaternion<T>(lhs) *= rhs;
    }

    /**
     * Rotates a vector by a quaternion.
     */
    template<typename T>
    constexpr Vec3<T> operator*(const Quaternion<T>& lhs, const Vec3<T>& rhs)
    {
        Vec3<T> u(lhs.x(), lhs.y(), lhs.z());
        return 2 * gml::dot(u, rhs) * u
               + (lhs.w() * lhs.w() - gml::dot(u, u)) * rhs
               + 2 * lhs.w() * gml::cross(u, rhs);
    }

    /**
     * Scales a quaternion by a scalar.
     */
    template<typename T>
    constexpr Quaternion<T> operator*(const Quaternion<T>& lhs, T rhs)
    {
        return Quaternion<T>(lhs) *= rhs;
    }

    /**
     * Scales a quaternion by a scalar.
     */
    template<typename T>
    constexpr Quaternion<T> operator*(T lhs, const Quaternion<T>& rhs)
    {
        return Quaternion<T>(rhs) *= lhs;
    }

    /**
     * Calculates the squared length of a quaternion.
     *
     * @param q The quaternion of which to calculate the length.
     * @return The quaternion's length squared.
     */
    template<typename T>
    constexpr T length_sqr(const Quaternion<T>& q)
    {
        return q.x() * q.x() + q.y() * q.y() + q.z() * q.z() + q.w() * q.w();
    }

    /**
     * Calculates the length of a quaternion.
     *
     * @param q The quaternion of which to calculate the length.
     * @return The quaternion's length.
     */
    template<std::floating_point T>
    constexpr T length(const Quaternion<T>& q)
    {
        return std::sqrt(length_sqr(q));
    }

    /**
     * Returns the normalization of a quaternion.
     *
     * @param q The quaternion to normalize.
     * @return A normalized copy of the quaternion.
     */
    template<std::floating_point T>
    constexpr Quaternion<T> normalize(Quaternion<T> q)
    {
        return q.normalize();
    }

    /**
     * Returns the conjugate of a quaternion.
     *
     * @param q The quaternion to conjugate.
     * @return A conjugate copy of the quaternion.
     */
    template<typename T>
    constexpr Quaternion<T> conjugate(Quaternion<T> q)
    {
        return q.conjugate();
    }
}

namespace gml::quaternion
{
    /**
     * Constructs a quaternion from an axis-angle representation.
     *
     * @param axis The rotation axis.
     * @param angle The rotation angle around the rotation axis in radians.
     * @return A quaternion representing the same rotation as the given axis-angle.
     */
    template<std::floating_point T>
    constexpr Quaternion<T> axis_angle(const Vec3<T>& axis, const T angle)
    {
        const Vec3<T> a = gml::normalize(axis);
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
    constexpr Quaternion<T> from_matrix(const Mat3<T>& matrix)
    {
        const T tr = trace(matrix);
        Mat3<T>& m = matrix;
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

    // TODO: look_at
}
