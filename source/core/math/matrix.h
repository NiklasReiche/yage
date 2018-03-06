#pragma once

#include "matrixBase.h"
#include "vector.h"
#include "quaternion.h"

namespace gml
{
	template <typename T>
	class Mat1;
	template <typename T>
	class Mat2;
	template <typename T>
	class Mat3;
	template <typename T>
	class Mat4;

	template <typename T, size_t Rows, size_t Columns>
	using Matrix = MatrixBase<T, Rows, Columns>;

	typedef Mat1<int> Mat1i;
	typedef Mat2<int> Mat2i;
	typedef Mat3<int> Mat3i;
	typedef Mat4<int> Mat4i;

	typedef Mat1<unsigned int> Mat1ui;
	typedef Mat2<unsigned int> Mat2ui;
	typedef Mat3<unsigned int> Mat3ui;
	typedef Mat4<unsigned int> Mat4ui;

	typedef Mat1<float> Mat1f;
	typedef Mat2<float> Mat2f;
	typedef Mat3<float> Mat3f;
	typedef Mat4<float> Mat4f;

	typedef Mat1<double> Mat1d;
	typedef Mat2<double> Mat2d;
	typedef Mat3<double> Mat3d;
	typedef Mat4<double> Mat4d;


	template <typename T>
	class Mat1 : public MatrixBase<T, 1, 1>
	{
	public:
		/** Inherit Constructors from MatrixBase */
		using MatrixBase<T, 1, 1>::MatrixBase;

		Mat1();

		Mat1(const Mat1<T>& other);

		Mat1(const MatrixBase<T, 1, 1>& other);
	};

	template <typename T>
	class Mat2 : public MatrixBase<T, 2, 2>
	{
	public:
		/** Inherit Constructors from MatrixBase */
		using MatrixBase<T, 2, 2>::MatrixBase;

		Mat2();

		Mat2(const Mat2<T>& other);

		Mat2(const MatrixBase<T, 2, 2>& other);
	};

	template <typename T>
	class Mat3 : public MatrixBase<T, 3, 3>
	{
	public:
		/** Inherit Constructors from MatrixBase */
		using MatrixBase<T, 3, 3>::MatrixBase;

		Mat3();

		Mat3(const Mat3<T>& other);

		Mat3(const MatrixBase<T, 3, 3>& other);
	};

	template <typename T>
	class Mat4 : public MatrixBase<T, 4, 4>
	{
	public:
		/** Inherit Constructors from MatrixBase */
		using MatrixBase<T, 4, 4>::MatrixBase;

		Mat4();

		Mat4(const Mat4<T>& other);

		Mat4(const MatrixBase<T, 4, 4>& other);

	public:
		/**
		 * @brief Returns the translation component of this matrix.
		 * 
		 * @return the translation vector
		 */
		Vec3<T> getTranslation();

		/**
		 * @brief Returns the rotation component of this matrix as a matrix.
		 * 
		 * @return the rotation matrix
		 */
		Mat3<T> getRotation();

	public:
		/**
		 * @brief Constructs a translation matrix.
		 * 
		 * @param x the x component
		 * @param y the y component
		 * @param z the z component
		 * @return the translation matrix
		 */
		static Mat4<T> translate(double x, double y, double z);

		/**
		 * @brief Constructs a translation matrix from a single value.
		 * 
		 * @param translation the translation value
		 * @return the translation matrix
		 */
		static Mat4<T> translate(const double translation);

		/**
		 * @brief Constructs a translation matrix from a vector.
		 * 
		 * @param translation the translation vector
		 * @return the translation matrix
		 */
		static Mat4<T> translate(const Vec3<T>& translation);

		/**
		 * @brief Constructs a scaling matrix.
		 * 
		 * @param x the x component
		 * @param y the y component
		 * @param z the z component
		 * @return the scaling matrix
		 */
		static Mat4<T> scale(const double x, const double y, const double z);

		/**
		 * @brief Constructs a scaling matrix from a single value.
		 * 
		 * @param scale the scaling factor
		 * @return the scaling matrix
		 */
		static Mat4<T> scale(const double scale);

		/**
		 * @brief Constructs a scaling matrix from a vector.
		 * 
		 * @param scale the scaling vector
		 * @return the scaling matrix
		 */
		static Mat4<T> scale(const Vec3<T>& scale);

		/**
		 * @brief Constructs a rotation matrix from an axis angle representation.
		 * 
		 * @param axis the rotation axis
		 * @param angle the angle in degrees
		 * @return the rotation matrix
		 */
		static Mat4<T> axisAngle(Vec3<T> axis, double angle);

		/**
		 * @brief Constructs a rotation matrix from a quaternion representation.
		 * 
		 * @return the rotation matrix
		 */
		static Mat4<T> quaternion(Quaternion<T> quaternion);

		/**
		 * @brief Constructs a perspectiv view matrix.
		 * 
		 * @param fov the field of view in degrees
		 * @param aspect the aspect ration
		 * @param near the near plane
		 * @param far the far plane
		 * @return the view matrix
		 */
		static Mat4<T> perspective(
			const double fov, const double aspect,
			const double near, const double far);

		/**
		 * @brief Constructs an orthographic view matrix.
		 * 
		 * @param left the left plane
		 * @param right the right plane
		 * @param bottom the bottom plane
		 * @param top the top plane
		 * @param near the near plane
		 * @param far the far plane
		 * @return the view matrix
		 */
		static Mat4<T> orthographic(
			const double left, const double right,
			const double bottom, const double top,
			const double near, const double far);

		/**
		 * @brief Constructs a lookAt view matrix.
		 * 
		 * @param pos the camera position
		 * @param target the view target position
		 * @param up the camera up vector
		 * @return the view matrix
		 */
		static Mat4<T> lookAt(
			const Vec3<T>& pos,
			const Vec3<T>& target,
			const Vec3<T>& up);
	};
}

#include "matrix.tpp"
