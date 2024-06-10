#pragma once

#include <array>
#include <ostream>
#include <span>

#include "constraints.h"
#include "exception.h"
#include "maths.h"
#include "vector.h"
#include "quaternion.h"

namespace gml
{
    template<typename T>
    class Quaternion;

    template<typename T, std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
    class Matrix;

    template<std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
    using Mati = Matrix<int, M, N>;
    template<std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
    using Matf = Matrix<float, M, N>;
    template<std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
    using Matd = Matrix<double, M, N>;

    template<typename T>
    using Mat2 = Matrix<T, 2, 2>;

    using Mat2i = Mat2<int>;
    using Mat2f = Mat2<float>;
    using Mat2d = Mat2<double>;

    template<typename T>
    using Mat3 = Matrix<T, 3, 3>;

    using Mat3i = Mat3<int>;
    using Mat3f = Mat3<float>;
    using Mat3d = Mat3<double>;

    template<typename T>
    using Mat4 = Matrix<T, 4, 4>;

    using Mat4i = Mat4<int>;
    using Mat4f = Mat4<float>;
    using Mat4d = Mat4<double>;

    /**
     * Represents a generic NxM dimensional matrix. The dimensions must be greater than zero.
     *
     * @tparam T The type of the matrix's components.
     * @tparam M The number of rows.
     * @tparam N The number of columns.
     */
    template<typename T, std::size_t M, std::size_t N> requires StrictlyPositive<M> && StrictlyPositive<N>
    class Matrix
    {
    public:
        /**
         * Value-initializes all components.
         */
        constexpr Matrix()
            : m_elements{}
        {
        }

        /**
         * Initializes all components with the same value.
         *
         * @param value The default value used for initialization.
         */
        constexpr explicit Matrix(const T& value)
        {
            m_elements.fill(value);
        }

        /**
         * Initializes the matrix components from a variadic argument list.
         */
        template<typename... Args> requires (std::same_as<T, Args> && ...) && (M * N > 1)
        constexpr explicit Matrix(Args... args)
        {
            static_assert(sizeof...(args) == M * N);
            int i = 0;
            ((m_elements[i++] = args), ...);
        }

        /**
         * Initializes the components from a continuous memory block.
         * The data is interpreted in row-major format.
         *
         * @param data Continuous memory to copy elements from.
         */
        constexpr explicit Matrix(std::span<T, M * N> data)
        {
            std::ranges::copy(data, m_elements.begin());
        }

        /**
         * Casts this matrix to a matrix of a different component type.
         */
        template<typename T_>
        constexpr explicit operator Matrix<T_, M, N>() const
        {
            Matrix<T_, M, N> result;
            for (std::size_t m = 0; m < M; ++m) {
                for (std::size_t n = 0; n < N; ++n) {
                    result(m, n) = static_cast<T_>((*this)(m, n));
                }
            }
            return result;
        }

        /**
         * Gives direct access to the underlying array.
         *
         * @return Pointer to the underlying element storage (equal to the address of the first element).
         */
        constexpr T* data() noexcept
        {
            return m_elements.data();
        }

        /**
         * Gives direct access to the underlying array.
         *
         * @return Pointer to the underlying element storage (equal to the address of the first element).
         */
        constexpr const T* data() const noexcept
        {
            return m_elements.data();
        }

        /**
         * Returns the reference to an element of this matrix.
         *
         * @param m The row index of the element.
         * @param n The column index of the element.
         */
        constexpr T& operator()(const std::size_t m, const std::size_t n)
        {
            const std::size_t i = m * N + n;
            assert(i < M * N);
            return m_elements[i];
        }

        /**
         * Returns the reference to an element of this matrix.
         *
         * @param m The row index of the element.
         * @param n The column index of the element.
         */
        constexpr const T& operator()(const std::size_t m, const std::size_t n) const
        {
            const std::size_t i = m * N + n;
            assert(i < M * N);
            return m_elements[i];
        }

        /**
		 * Unary component-wise negation.
		 */
        constexpr Matrix operator-() const
        {
            Matrix result;
            for (std::size_t i = 0; i < M * N; ++i) {
                result.m_elements[i] = -m_elements[i];
            }
            return result;
        }

        /**
         * Component-wise addition.
         */
        constexpr Matrix& operator+=(const Matrix& rhs)
        {
            for (std::size_t i = 0; i < M * N; ++i) {
                m_elements[i] += rhs.m_elements[i];
            }
            return *this;
        }

        /**
         * Component-wise subtraction.
         */
        constexpr Matrix& operator-=(const Matrix& rhs)
        {
            for (std::size_t i = 0; i < M * N; ++i) {
                m_elements[i] -= rhs.m_elements[i];
            }
            return *this;
        }

        /**
         * Component-wise scalar multiplication.
         */
        constexpr Matrix& operator*=(const T& rhs)
        {
            for (std::size_t i = 0; i < M * N; ++i) {
                m_elements[i] *= rhs;
            }
            return *this;
        }

        /**
         * Matrix multiplication with a square matrix.
         */
        constexpr Matrix& operator*=(const Matrix<T, N, N>& rhs)
        {
            return operator=(*this * rhs);
        }

        /**
         * Element-wise scalar division. Results in undefined behaviour if passed zero.
         */
        constexpr Matrix& operator/=(const T& rhs)
        {
            for (std::size_t i = 0; i < M * N; ++i) {
                m_elements[i] /= rhs;
            }
            return *this;
        }

        /**
        * Switches two rows in this matrix.
        *
        * @param first The index of the first row to switch with the other row.
        * @param second The index of the second row to switch with the other row.
        */
        constexpr void switch_rows(const std::size_t first, const std::size_t second)
        {
            for (std::size_t i = 0; i < N; ++i) {
                T temp = (*this)(first, i);
                (*this)(first, i) = (*this)(second, i);
                (*this)(second, i) = temp;
            }
        }

        /**
         * Adds a multiplied row to another multiplied row in this matrix.
         * The row is modified by the following equation: rowA = a * rowA + b * rowB
         *
         * @param row_a The index of the row to modify.
         * @param a The factor by which to multiply rowA.
         * @param row_b The index of the row to add to the modified rowA.
         * @param b The factor by which to multiply rowB before adding it to rowA.
         */
        constexpr void combine_rows(const std::size_t row_a, double a, const std::size_t row_b, double b)
        {
            for (std::size_t i = 0; i < N; ++i) {
                (*this)(row_a, i) = (*this)(row_a, i) * a + (*this)(row_b, i) * b;
            }
        }

        /**
        * @return The translation component of this matrix.
        */
        constexpr Vector<T, 3> translation() const requires (N == 4 && M == 4)
        {
            return Vector<T, 3>{(*this)(0, 3), (*this)(1, 3), (*this)(2, 3)};
        }

        /**
        * @return The rotation component of this matrix.
        */
        constexpr Matrix<T, 3, 3> rotation() const requires (N == 4 && M == 4)
        {
            const Vector<T, 3> scale = scale();
            return Matrix<T, 3, 3>{
                (*this)(0, 0) / scale.x(), (*this)(0, 1) / scale.y(), (*this)(0, 2) / scale.z(),
                (*this)(1, 0) / scale.x(), (*this)(1, 1) / scale.y(), (*this)(1, 2) / scale.z(),
                (*this)(2, 0) / scale.x(), (*this)(2, 1) / scale.y(), (*this)(2, 2) / scale.z()
            };
        }

        /**
        * @return The scaling component of this matrix.
        */
        constexpr Vector<T, 3> scale() const requires (N == 4 && M == 4)
        {
            return Vector<T, 3>{
                length(Vector<T, 3>{(*this)(0, 0), (*this)(1, 0), (*this)(2, 0)}),
                length(Vector<T, 3>{(*this)(0, 1), (*this)(1, 1), (*this)(2, 1)}),
                length(Vector<T, 3>{(*this)(0, 2), (*this)(1, 2), (*this)(2, 2)})
            };
        }

        template<typename T_, std::size_t M_, std::size_t N_>
        friend constexpr bool operator==(const Matrix<T_, M_, N_>& left, const Matrix<T_, M_, N_>& right);

        template<typename T_, std::size_t M_, std::size_t N_>
        friend constexpr bool operator!=(const Matrix<T_, M_, N_>& left, const Matrix<T_, M_, N_>& right);

    private:
        /**
         * The matrix components.
         */
        std::array<T, N * M> m_elements;
    };

    template<typename T, std::size_t M, std::size_t N>
    std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& matrix)
    {
        os << "\n[ ";
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                os << (i == 0 && j == 0 ? "" : "  ") << (matrix(i, j) < 0 ? "" : " ") << matrix(i, j);
            }
            os << (i < M - 1 ? "\n" : " ]\n");
        }
        return os;
    }

    /**
     * Component-wise comparison.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr bool operator==(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
    {
        return left.m_elements == right.m_elements;
    }

    /**
     * Component-wise comparison.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr bool operator!=(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
    {
        return left.m_elements != right.m_elements;
    }

    /**
     * Component-wise addition.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr Matrix<T, M, N> operator+(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
    {
        return Matrix<T, M, N>(left) += right;
    }

    /**
     * Component-wise subtraction.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr Matrix<T, M, N> operator-(const Matrix<T, M, N>& left, const Matrix<T, M, N>& right)
    {
        return Matrix<T, M, N>(left) -= right;
    }

    /**
     * Standard matrix multiplication.
     */
    template<typename T, std::size_t M, std::size_t N, std::size_t P>
    constexpr Matrix<T, M, P> operator*(const Matrix<T, M, N>& left, const Matrix<T, N, P>& right)
    {
        Matrix<T, M, P> result(0);
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t k = 0; k < N; ++k) {
                for (std::size_t j = 0; j < P; ++j){ // loops switched for cache locality
                    result(i, j) += left(i, k) * right(k, j);
                }
            }
        }
        return result;
    }

    /**
     * Scalar multiplication.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr Matrix<T, M, N> operator*(const T& left, const Matrix<T, M, N>& right)
    {
        return Matrix<T, M, N>(right) *= left;
    }

    /**
     * Scalar multiplication.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr Matrix<T, M, N> operator*(const Matrix<T, M, N>& left, const T& right)
    {
        return Matrix<T, M, N>(left) *= right;
    }

    /**
     * Standard matrix-vector multiplication.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr Vector<T, M> operator*(const Matrix<T, M, N>& left, const Vector<T, N>& right)
    {
        Vector<T, M> result(0);
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                result(i) += left(i, j) * right(j);
            }
        }
        return result;
    }

    /**
     * Scalar division.
     */
    template<typename T, std::size_t M, std::size_t N>
    constexpr Matrix<T, M, N> operator/(const Matrix<T, M, N>& left, const T& right)
    {
        return Matrix<T, M, N>(left) /= right;
    }

    namespace matrix
    {
        /**
          * The NxN identity matrix.
          *
          * @tparam T The element type of the matrix.
          * @tparam N The dimension of the identity matrix.
          */
        template<typename T, std::size_t N>
        inline constexpr auto Id = []() constexpr {
            Matrix<T, N, N> m;
            for (std::size_t i = 0; i < N; ++i) {
                m(i, i) = 1;
            }
            return m;
        }();

        template<typename T>
        inline constexpr auto& Id1 = Id<T, 1>;

        template<typename T>
        inline constexpr auto& Id2 = Id<T, 2>;
        inline constexpr auto& Id2f = Id<float, 2>;
        inline constexpr auto& Id2d = Id<double, 2>;

        template<typename T>
        inline constexpr auto& Id3 = Id<T, 3>;
        inline constexpr auto& Id3f = Id<float, 3>;
        inline constexpr auto& Id3d = Id<double, 3>;

        template<typename T>
        inline constexpr auto& Id4 = Id<T, 4>;
        inline constexpr auto& Id4f = Id<float, 4>;
        inline constexpr auto& Id4d = Id<double, 4>;
    }

    /**
     * Returns a matrix with a given row and column removed.
     * Results in undefined behaviour if the row or column are out of bounds.
     *
     * @param matrix The matrix to reduce.
     * @param row The index of the row to remove.
     * @param column The index of the column to remove.
     * @return The reduced matrix.
     */
    template<typename T, std::size_t M, std::size_t N>
    [[nodiscard]]
    constexpr Matrix<T, M - 1, N - 1>
    reduce(const Matrix<T, M, N>& matrix, const std::size_t row, const std::size_t column)
    {
        assert(row < M && column < N);

        Matrix<T, M - 1, N - 1> result(0);
        for (std::size_t i = 0; i < M - 1; ++i) {
            const std::size_t row_i = i < row ? i : i + 1;
            for (std::size_t j = 0; j < N - 1; ++j) {
                const std::size_t col_i = j < column ? j : j + 1;
                result(i, j) = matrix(row_i, col_i);
            }
        }
        return result;
    }

    /**
     * @return The determinant of the given quadratic matrix.
     */
    template<typename T, std::size_t N>
    [[nodiscard]]
    constexpr T det(const Matrix<T, N, N>& matrix)
    {
        T result = 0;
        for (std::size_t i = 0; i < N; ++i) {
            const T coeff = (i + 1) % 2 == 0 ? 1 : -1;
            result += coeff * matrix(1, i) * det(reduce(matrix, 1, i));
        }
        return result;
    }

    /**
     * @return The determinant of the given 1x1 matrix.
     */
    template<typename T>
    [[nodiscard]]
    constexpr T det(const Matrix<T, 1, 1>& matrix)
    {
        return matrix(0, 0);
    }

    /**
     * @return The determinant of the given 2x2 matrix.
     */
    template<typename T>
    [[nodiscard]]
    constexpr T det(const Matrix<T, 2, 2>& matrix)
    {
        return matrix(0, 0) * matrix(1, 1) - matrix(0, 1) * matrix(1, 0);
    }

    /**
     * @return The determinant of the given 3x3 matrix.
     */
    template<typename T>
    [[nodiscard]]
    constexpr T det(const Matrix<T, 3, 3>& matrix)
    {
        return matrix(0, 0) * matrix(1, 1) * matrix(2, 2) +
               matrix(0, 1) * matrix(1, 2) * matrix(2, 0) +
               matrix(0, 2) * matrix(1, 0) * matrix(2, 1) -
               matrix(0, 2) * matrix(1, 1) * matrix(2, 0) -
               matrix(0, 1) * matrix(1, 0) * matrix(2, 2) -
               matrix(0, 0) * matrix(1, 2) * matrix(2, 1);
    }

    /**
     * @return The trace of the given quadratic matrix.
     */
    template<typename T, std::size_t N>
    [[nodiscard]]
    constexpr T trace(const Matrix<T, N, N>& matrix)
    {
        T trace = 0;
        for (std::size_t i = 0; i < N; ++i) {
            trace += matrix(i, i);
        }
        return trace;
    }

    /**
     * Calculates the transpose of a matrix.
     *
     * @param matrix The matrix for which to calculate the transpose.
     * @return The matrix's transpose matrix.
     */
    template<typename T, std::size_t M, std::size_t N>
    [[nodiscard]]
    constexpr Matrix<T, N, M> transpose(const Matrix<T, M, N>& matrix)
    {
        Matrix<T, N, M> result(0);
        for (std::size_t i = 0; i < M; ++i) {
            for (std::size_t j = 0; j < N; ++j) {
                result(j, i) = matrix(i, j);
            }
        }
        return result;
    }

    /**
     * Calculates the inverse of a square matrix.
     *
     * Uses the Gauss-Jordan Elimination algorithm. If the matrix is not invertible an exception is thrown.
     *
     * @param matrix The matrix for which to calculate the inverse.
     * @return The matrix's inverse matrix.
     *
     * @throws DivideByZeroException The determinant of the matrix is zero.
     */
    template<typename T, std::size_t N>
    [[nodiscard]]
    constexpr Matrix<T, N, N> inverse(const Matrix<T, N, N>& matrix)
    {
        if (det(matrix) == 0) {
            throw DivideByZeroException();
        }

        Matrix<T, N, N> mat = matrix;
        Matrix<T, N, N> inv = gml::matrix::Id<T, N>;

        auto pivotSearch = [&mat](std::size_t i_min) {
            std::size_t i_max = i_min;
            for (std::size_t i = i_min + 1; i < N; ++i) {
                if (std::abs(mat(i, i_min)) > std::abs(mat(i_max, i_min))) {
                    i_max = i;
                }
            }
            return i_max;
        };

        // transform to lower triangular form
        for (std::size_t i = 0; i < N; ++i) {
            // column pivot search for numeric stability
            auto i_pivot = pivotSearch(i);
            if (mat(i_pivot, i) == 0) {
                throw DivideByZeroException();
            }
            if (i_pivot != i) {
                mat.switch_rows(i_pivot, i);
                inv.switch_rows(i_pivot, i);
            }

            // normalize the pivot
            T value = mat(i, i);
            mat.combine_rows(i, 1 / value, 0, 0);
            inv.combine_rows(i, 1 / value, 0, 0);

            // reduce remaining column to zero
            for (std::size_t j = i + 1; j < N; ++j) {
                T factor = -mat(j, i);
                mat.combine_rows(j, 1, i, factor);
                inv.combine_rows(j, 1, i, factor);
            }
        }

        // reduce values above diagonal to zero
        for (std::size_t i = N; i-- > 1;) // prevent unsigned overflow
        {
            for (std::size_t j = i; j-- > 0;) // prevent unsigned overflow
            {
                T factor = -mat(j, i);
                mat.combine_rows(j, 1, i, factor);
                inv.combine_rows(j, 1, i, factor);
            }
        }

        return inv;
    }

    template<typename T>
    [[nodiscard]]
    constexpr Matrix<T, 2, 2> inverse(const Matrix<T, 2, 2>& matrix)
    {
        const double m_det = det(matrix);
        if (m_det == 0) {
            throw DivideByZeroException();
        }

        Matrix<T, 2, 2> inv{
            matrix(1, 1), -matrix(0, 1),
            -matrix(1, 0), matrix(0, 0)
        };
        return inv / m_det;
    }

    template<typename T>
    [[nodiscard]]
    constexpr Matrix<T, 3, 3> inverse(const Matrix<T, 3, 3>& matrix)
    {
        const double m_det = det(matrix);
        if (m_det == 0) {
            throw DivideByZeroException();
        }

        auto& m = matrix;
        Matrix<T, 3, 3> inv{
            m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1), m(0, 2) * m(2, 1) - m(0, 1) * m(2, 2),
            m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1),
            m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2), m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0),
            m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2),
            m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0), m(0, 1) * m(2, 0) - m(0, 0) * m(2, 1),
            m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)
        };
        return inv / m_det;
    }
}

namespace gml::matrix
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
        Vec3<T> a = gml::normalize(axis);
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
    template<typename T>
    Mat4<T> look_at(const Vec3<T>& pos, const Vec3<T>& target, const Vec3<T>& up)
    {
        Vec3<T> direction = normalize(pos - target);
        Vec3<T> right = normalize(cross(up, direction));
        Mat4<T> mat1 = {
            {right.x(), right.y(), right.z(), 0},
            {up.x(), up.y(), up.z(), 0},
            {direction.x(), direction.y(), direction.z(), 0},
            {0, 0, 0, 1}
        };
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
    template<typename T>
    Mat4<T> look_at(const Vec3<T>& pos, const Vec3<T>& target)
    {
        Vec3<T> direction = normalize(pos - target);
        Vec3<T> right = normalize(cross(vector::worldUp<T>(), direction));
        Vec3<T> up = normalize(cross(direction, right));
        Mat4<T> mat1 = {
            {right.x(), right.y(), right.z(), 0},
            {up.x(), up.y(), up.z(), 0},
            {direction.x(), direction.y(), direction.z(), 0},
            {0, 0, 0, 1}
        };
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

    // TODO: euler_angle()
}
