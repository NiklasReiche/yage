#pragma once

namespace gml
{
	template <typename T>
	Mat1<T>::Mat1()
		: MatrixBase()
	{
	}

	template <typename T>
	Mat1<T>::Mat1(const Mat1<T>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat1<T>::Mat1(const MatrixBase<T, 1, 1>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat2<T>::Mat2()
		: MatrixBase()
	{
	}

	template <typename T>
	Mat2<T>::Mat2(const Mat2<T>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat2<T>::Mat2(const MatrixBase<T, 2, 2>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat3<T>::Mat3()
		: MatrixBase()
	{
	}

	template <typename T>
	Mat3<T>::Mat3(const Mat3<T>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat3<T>::Mat3(const MatrixBase<T, 3, 3>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat4<T>::Mat4()
		: MatrixBase()
	{
	}

	template <typename T>
	Mat4<T>::Mat4(const Mat4<T>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Mat4<T>::Mat4(const MatrixBase<T, 4, 4>& other)
		: MatrixBase(other)
	{
	}

	template <typename T>
	Vec3<T> Mat4<T>::getTranslation()
	{
		return Vec3<T>(this->at(0, 3), this->at(1, 3), this->at(2, 3));
	}

	template <typename T>
	Mat3<T> Mat4<T>::getRotation()
	{
		return Mat3<T>(
			this->at(0, 0), this->at(0, 1), this->at(0, 2),
			this->at(1, 0), this->at(1, 1), this->at(1, 2),
			this->at(2, 0), this->at(2, 1), this->at(2, 2)
		);
	}

	template <typename T>
	Mat4<T> Mat4<T>::translate(const double x, const double y, const double z)
	{
		Mat4<T> result;
		result.at(0, 3) = x;
		result.at(1, 3) = y;
		result.at(2, 3) = z;
		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::translate(const double translation)
	{
		return Mat4<T>::translate(translation, translation, translation);
	}

	template <typename T>
	Mat4<T> Mat4<T>::translate(const Vec3<T>& translation)
	{
		return Mat4<T>::translate(
			translation.at(0), translation.at(1), translation.at(2));
	}

	template <typename T>
	Mat4<T> Mat4<T>::scale(const double x, const double y, const double z)
	{
		Mat4<T> result;
		result.at(0, 0) = x;
		result.at(1, 1) = y;
		result.at(2, 2) = z;
		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::scale(const double scale)
	{
		return Mat4<T>::scale(scale, scale, scale);
	}

	template <typename T>
	Mat4<T> Mat4<T>::scale(const Vec3<T>& scale)
	{
		return Mat4<T>::scale(scale.at(0), scale.at(1), scale.at(2));
	}

	template <typename T>
	Mat4<T> Mat4<T>::axisAngle(Vec3<T> axis, double angle)
	{
		// TODO
		return Mat4<T>();
	}

	template <typename T>
	Mat4<T> Mat4<T>::quaternion(Quaternion<T> quaternion)
	{
		// TODO
		return Mat4<T>();
	}

	template <typename T>
	Mat4<T> Mat4<T>::perspective(
		const double fov, const double aspect,
		const double near, const double far)
	{
		Mat4<T> result;
		const double top = near * std::tan(toRad(fov / 2));
		const double bottom = 0 - top;
		const double right = top * aspect;
		const double left = 0 - right;
		result.at(0, 0) = (2 * near) / (right - left);
		result.at(0, 2) = (right + left) / (right - left);
		result.at(1, 1) = (2 * near) / (top - bottom);
		result.at(1, 2) = (top + bottom) / (top - bottom);
		result.at(2, 2) = 0 - ((far + near) / (far - near));
		result.at(2, 3) = 0 - ((2 * far * near) / (far - near));
		result.at(3, 2) = -1;
		result.at(3, 3) = 0;
		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::orthographic(
		const double left, const double right,
		const double bottom, const double top,
		const double near, const double far)
	{
		Mat4<T> result;
		result.at(0, 0) = 2 / (right - left);
		result.at(0, 3) = -((right + left) / (right - left));
		result.at(1, 1) = 2 / (top - bottom);
		result.at(1, 3) = -((top + bottom) / (top - bottom));
		result.at(2, 2) = -2 / (far - near);
		result.at(2, 3) = -((far + near) / (far - near));
		result.at(3, 3) = 1;
		return result;
	}

	template <typename T>
	Mat4<T> Mat4<T>::lookAt(
		const Vec3<T>& pos,
		const Vec3<T>& target,
		const Vec3<T>& up)
	{
		Vec3<T> direction = pos - target;
		Vec3<T> right = normalize(cross(up, direction));
		Mat4<T> mat1 = {
			{right.x, right.y, right.z, 0,},
			{up.x, up.y, up.z, 0,},
			{direction.x, direction.y, direction.z, 0,},
			{0, 0, 0, 1}
		};
		return mat1 * Mat4<T>::translate(-pos.x, -pos.y, -pos.z);
	}
}
