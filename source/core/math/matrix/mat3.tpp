#pragma once

namespace gml
{
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
	Quaternion<T> Mat3<T>::toQuaternion()
	{
		return Quaternion<T>::rotationMatrix(*this);
	}
}
