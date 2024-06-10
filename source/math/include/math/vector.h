#pragma once

#include <array>
#include <cassert>
#include <ostream>
#include <span>
#include <algorithm>

#include "constraints.h"
#include "maths.h"

namespace yage::math
{
    template<typename T, std::size_t Size> requires StrictlyPositive<Size>
    class Vector;

    template<std::size_t Size> requires StrictlyPositive<Size>
    using Veci = Vector<int, Size>;
    template<std::size_t Size> requires StrictlyPositive<Size>
    using Vecf = Vector<float, Size>;
    template<std::size_t Size> requires StrictlyPositive<Size>
    using Vecd = Vector<double, Size>;

    template<typename T>
    using Vec2 = Vector<T, 2>;

    using Vec2i = Vec2<int>;
    using Vec2ui = Vec2<unsigned int>;
    using Vec2f = Vec2<float>;
    using Vec2d = Vec2<double>;

    template<typename T>
    using Vec3 = Vector<T, 3>;

    using Vec3i = Vec3<int>;
    using Vec3ui = Vec3<unsigned int>;
    using Vec3f = Vec3<float>;
    using Vec3d = Vec3<double>;

    template<typename T>
    using Vec4 = Vector<T, 4>;

    using Vec4i = Vec4<int>;
    using Vec4ui = Vec4<unsigned int>;
    using Vec4f = Vec4<float>;
    using Vec4d = Vec4<double>;

    template<std::floating_point T, std::size_t Size>
    constexpr T length(const Vector<T, Size>& vector);

    /**
     * Represents a generic n-dimensional vector.
     *
     * @tparam T The type of the vector's components.
     * @tparam Size The vector's dimension.
     */
    template<typename T, std::size_t Size> requires StrictlyPositive<Size>
    class Vector
    {
    public:
        /**
         * Value-initializes all vector components.
         */
        constexpr Vector()
            : m_elements{}
        {
        }

        /**
         * Initializes all vector components with the same value.
         *
         * @param value The initializing value.
         */
        constexpr explicit Vector(T value)
        {
            m_elements.fill(value);
        }

        constexpr Vector(T x, T y) requires (Size == 2)
            : m_elements{x, y}
        {
        }

        constexpr Vector(T x, T y, T z) requires (Size == 3)
            : m_elements{x, y, z}
        {
        }

        constexpr Vector(T x, T y, T z, T w) requires (Size == 4)
            : m_elements{x, y, z, w}
        {
        }

        template<typename... Args> requires (std::same_as<T, Args> && ...) && (Size > 4)
        constexpr explicit Vector(Args... args)
        {
            static_assert(sizeof...(args) == Size);
            int i = 0;
            ((m_elements[i++] = args), ...);
        }

        /**
         * Initializes the components from a continuous memory block.
         *
         * @param data Continuous memory to copy elements from.
         */
        constexpr explicit Vector(std::span<T, Size> data)
        {
            std::ranges::copy(data, m_elements.begin());
        }

        /**
         * Casts this vector to a vector of a different component type.
         */
        template<typename T_>
        constexpr explicit operator Vector<T_, Size>() const
        {
            Vector<T_, Size> result;
            for (std::size_t i = 0; i < Size; ++i) {
                result(i) = static_cast<T_>(m_elements[i]);
            }
            return result;
        }

        /**
         * Returns a reference to this vector's component at a given index.
         *
         * @param i The components's index.
         * @return A reference to the component.
         */
        constexpr T& operator()(int i)
        {
            assert(i >= 0 && static_cast<std::size_t>(i) < Size);
            return m_elements[i];
        }

        /**
         * Returns a const reference to this vector's component at a given index.
         *
         * @param i The component's index.
         * @return A const reference to the component.
         */
        constexpr const T& operator()(int i) const
        {
            assert(i >= 0 && static_cast<std::size_t>(i) < Size);
            return m_elements[i];
        }

        /**
         * Gives direct access to the underlying array.
         *
         * @return Pointer to the underlying element storage (equal to the address of the first element).
         */
        [[nodiscard]]
        constexpr T* data() noexcept
        {
            return m_elements.data();
        }

        /**
         * Gives direct access to the underlying array.
         *
         * @return Pointer to the underlying element storage (equal to the address of the first element).
         */
        [[nodiscard]]
        constexpr const T* data() const noexcept
        {
            return m_elements.data();
        }

        /**
         * @return A reference to the x-component.
         */
        [[nodiscard]]
        constexpr T& x() requires (Size >= 1 && Size <= 4)
        {
            return m_elements[0];
        }

        /**
         * @return A const reference to the x-component.
         */
        [[nodiscard]]
        constexpr const T& x() const requires (Size >= 1 && Size <= 4)
        {
            return m_elements[0];
        }

        /**
         * @return A reference to the y-component.
         */
        [[nodiscard]]
        constexpr T& y() requires (Size >= 2 && Size <= 4)
        {
            return m_elements[1];
        }

        /**
         * @return A const reference to the x-component.
         */
        [[nodiscard]]
        constexpr const T& y() const requires (Size >= 2 && Size <= 4)
        {
            return m_elements[1];
        }

        /**
         * @return A reference to the z-component.
         */
        [[nodiscard]]
        constexpr T& z() requires (Size >= 3 && Size <= 4)
        {
            return m_elements[2];
        }

        /**
         * @return A cost reference to the z-component.
         */
        [[nodiscard]]
        constexpr const T& z() const requires (Size >= 3 && Size <= 4)
        {
            return m_elements[2];
        }

        /**
         * @return A reference to the w-component.
         */
        [[nodiscard]]
        constexpr T& w() requires (Size == 4)
        {
            return m_elements[3];
        }

        /**
         * @return A const reference to the w-component.
         */
        [[nodiscard]]
        constexpr const T& w() const requires (Size == 4)
        {
            return m_elements[3];
        }

        /**
         * Normalizes this vector in-place.
         * Results in undefined behaviour if called on the null vector.
         */
        constexpr Vector& normalize()
        {
            const T len = length(*this);
            assert(len != 0);
            return operator/=(len);
        }

        /**
         * Unary component-wise negation.
         */
        constexpr Vector operator-() const
        {
            Vector result;
            for (std::size_t i = 0; i < Size; ++i) {
                result.m_elements[i] = -m_elements[i];
            }
            return result;
        }

        /**
         * Component-wise addition.
         */
        constexpr Vector& operator+=(const Vector& right)
        {
            for (std::size_t i = 0; i < Size; ++i) {
                m_elements[i] += right.m_elements[i];
            }
            return *this;
        }

        /**
         * Component-wise subtraction.
         */
        constexpr Vector& operator-=(const Vector& right)
        {
            for (std::size_t i = 0; i < Size; ++i) {
                m_elements[i] -= right.m_elements[i];
            }
            return *this;
        }

        /**
         * Component-wise scalar multiplication.
         */
        constexpr Vector& operator*=(const T& right)
        {
            for (std::size_t i = 0; i < Size; ++i) {
                m_elements[i] *= right;
            }
            return *this;
        }

        /**
         * Component-wise scalar division. Results in undefined behaviour if the given scalar is zero.
         */
        constexpr Vector& operator/=(const T& right)
        {
            assert(right != 0);
            for (std::size_t i = 0; i < Size; ++i) {
                m_elements[i] /= right;
            }
            return *this;
        }

    private:
        /**
        * The vector's components.
        */
        std::array<T, Size> m_elements;

        template<typename T_, std::size_t Size_>
        friend constexpr bool operator==(const Vector<T_, Size_>&, const Vector<T_, Size_>&);

        template<typename T_, std::size_t Size_>
        friend constexpr bool operator!=(const Vector<T_, Size_>&, const Vector<T_, Size_>&);
    };

    template<typename T, std::size_t Size>
    std::ostream& operator<<(std::ostream& os, const Vector<T, Size>& vec)
    {
        os << "[";
        for (std::size_t i = 0; i < Size; ++i) {
            os << vec(i) << (i == Size - 1 ? "" : " ");
        }
        os << "]";
        return os;
    }

    /**
     * Component-wise comparison.
     */
    template<typename T, std::size_t Size>
    constexpr bool operator==(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
    {
        return lhs.m_elements == rhs.m_elements;
    }

    /**
     * Component-wise comparison.
     */
    template<typename T, std::size_t Size>
    constexpr bool operator!=(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
    {
        return lhs.m_elements != rhs.m_elements;
    }

    /**
     * Component-wise addition.
     */
    template<typename T, std::size_t Size>
    constexpr Vector<T, Size> operator+(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
    {
        return Vector<T, Size>(lhs) += rhs;
    }

    /**
     * Component-wise subtraction.
     */
    template<typename T, std::size_t Size>
    constexpr Vector<T, Size> operator-(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
    {
        return Vector<T, Size>(lhs) -= rhs;
    }

    /**
     * Scalar multiplication.
     */
    template<typename T, std::size_t Size>
    constexpr Vector<T, Size> operator*(const T lhs, const Vector<T, Size>& rhs)
    {
        return Vector<T, Size>(rhs) *= lhs;
    }

    /**
     * Scalar multiplication.
     */
    template<typename T, std::size_t Size>
    constexpr Vector<T, Size> operator*(const Vector<T, Size>& lhs, const T rhs)
    {
        return Vector<T, Size>(lhs) *= rhs;
    }

    /**
     * Scalar division.
     */
    template<typename T, std::size_t Size>
    constexpr Vector<T, Size> operator/(const Vector<T, Size>& lhs, const T rhs)
    {
        return Vector<T, Size>(lhs) /= rhs;
    }

    /**
    * @return The cross product of two given 3-dimensional vectors.
    */
    template<typename T>
    constexpr Vector<T, 3> cross(Vector<T, 3> left, Vector<T, 3> right)
    {
        return {
            left.y() * right.z() - left.z() * right.y(),
            left.z() * right.x() - left.x() * right.z(),
            left.x() * right.y() - left.y() * right.x()
        };
    }

    /**
     * @return The dot product of two given vectors.
     */
    template<typename T, std::size_t Size>
    [[nodiscard]]
    constexpr T dot(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
    {
        T result = 0;
        for (std::size_t i = 0; i < Size; ++i) {
            result += lhs(i) * rhs(i);
        }
        return result;
    }

    /**
     * @return The squared euclidean length of the given vector.
     */
    template<typename T, std::size_t Size>
    [[nodiscard]]
    constexpr T length_sqr(const Vector<T, Size>& vector)
    {
        return dot(vector, vector);
    }

    /**
     * @return The euclidean length of the given vector.
     */
    template<std::floating_point T, std::size_t Size>
    [[nodiscard]]
    constexpr T length(const Vector<T, Size>& vector)
    {
        return std::sqrt(length_sqr(vector));
    }

    /**
     * Creates a normalized copy of the given vector.
     * Results in undefined behaviour if called with the null vector.
     */
    template<std::floating_point T, std::size_t Size>
    [[nodiscard]]
    constexpr Vector<T, Size> normalize(const Vector<T, Size>& vector)
    {
        const T len = length(vector);
        assert(len != 0);
        return vector / length(vector);
    }

    /**
     * Calculates the angle in radians between two given vectors.
     * Results in undefined behaviour if passed the null vector
     */
    template<std::floating_point T, std::size_t Size>
    [[nodiscard]]
    constexpr T angle_rad(const Vector<T, Size>& lhs, const Vector<T, Size>& rhs)
    {
        const T lengths = length(lhs) * length(rhs);
        assert(lengths != 0);
        return std::acos(dot(lhs, rhs) / lengths);
    }
}

namespace yage::math::vector
{
    template<typename T>
    constexpr Vec3<T> worldForward()
    {
        return Vec3<T>(0, 0, 1);
    }

    template<typename T>
    constexpr Vec3<T> worldUp()
    {
        return Vec3<T>(0, 1, 0);
    }

    template<typename T>
    constexpr Vec3<T> worldRight()
    {
        return Vec3<T>(-1, 0, 0); // TODO:
    }
}
