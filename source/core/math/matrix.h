#pragma once
#include <iostream>

#include "vector.h"
#include "quaternion.h"

namespace gml
{
	template <typename T>
	class Matrix4D
	{
	public:
		T value[4][4];

		Matrix4D()
		{
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == j) {
						value[i][j] = 1.0f;
					}
					else {
						value[i][j] = 0.0f;
					}
				}
			}
		}
		Matrix4D(T arr[])
		{
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					value[i][j] = arr[i * 4 + j];
				}
			}
		}

		void convertToArray(T arr[])
		{
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					arr[i * 4 + j] = this->value[i][j];
				}
			}
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, Matrix4D<T> mat)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				os << mat.value[i][j] << " ";
			}
			os << std::endl;
		}
		return os;
	}

	template <typename T>
	Matrix4D<T> operator+(Matrix4D<T> left, Matrix4D<T> right) {
		Matrix4D<T> out;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				out.value[i][j] = left.value[i][j] + right.value[i][j];
			}
		}
		return out;
	}
	template <typename T>
	Matrix4D<T> operator-(Matrix4D<T> left, Matrix4D<T> right) {
		Matrix4D<T> out;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				out.value[i][j] = left.value[i][j] - right.value[i][j];
			}
		}
		return out;
	}

	template <typename T>
	Matrix4D<T> operator*(Matrix4D<T> mat1, Matrix4D<T> mat2) {
		Matrix4D<T> mat3;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				mat3.value[i][j] = mat1.value[i][0] * mat2.value[0][j] + mat1.value[i][1] * mat2.value[1][j] + mat1.value[i][2] * mat2.value[2][j] + mat1.value[i][3] * mat2.value[3][j];
			}
		}
		return mat3;
	}
	template <typename T>
	Matrix4D<T> operator*(Matrix4D<T> mat, float s) {
		Matrix4D<T> res;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.value[i][j] = mat.value[i][j] * s;
			}
		}
		return res;
	}
	template <typename T>
	Matrix4D<T> operator*(float s, Matrix4D<T> mat) {
		Matrix4D<T> res;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				res.value[i][j] = s * mat.value[i][j];
			}
		}
		return res;
	}

	template <typename T>
	Vec3<T> operator*(Matrix4D<T> mat, Vec3<T> vec1) {
		Vec3<T> vec2;
		vec2.x = mat.value[0][0] * vec1.x + mat.value[0][1] * vec1.y + mat.value[0][2] * vec1.z;
		vec2.y = mat.value[1][0] * vec1.x + mat.value[1][1] * vec1.y + mat.value[1][2] * vec1.z;
		vec2.z = mat.value[2][0] * vec1.x + mat.value[2][1] * vec1.y + mat.value[2][2] * vec1.z;
		return vec2;
	}
	template <typename T>
	Vec4<T> operator*(Matrix4D<T> mat, Vec4<T> vec1) {
		Vec4<T> vec2;
		vec2.x = mat.value[0][0] * vec1.x + mat.value[0][1] * vec1.y + mat.value[0][2] * vec1.z + mat.value[0][3] * vec1.z;
		vec2.y = mat.value[1][0] * vec1.x + mat.value[1][1] * vec1.y + mat.value[1][2] * vec1.z + mat.value[1][3] * vec1.z;
		vec2.z = mat.value[2][0] * vec1.x + mat.value[2][1] * vec1.y + mat.value[2][2] * vec1.z + mat.value[2][3] * vec1.z;
		vec2.w = mat.value[3][0] * vec1.x + mat.value[3][1] * vec1.y + mat.value[3][2] * vec1.z + mat.value[3][3] * vec1.z;
		return vec2;
	}

	/*  Matrix Calculations */
	template <typename T>
	Matrix4D<T> strip(Matrix4D<T> m) {
		Matrix4D<T> out;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				out.value[i][j] = m.value[i][j];
			}
		}
		out.value[0][3] = 0.0f;
		out.value[1][3] = 0.0f;
		out.value[2][3] = 0.0f;
		out.value[3][3] = 1.0f;
		out.value[3][0] = 0.0f;
		out.value[3][1] = 0.0f;
		out.value[3][2] = 0.0f;

		return out;
	}

	template <typename T>
	Matrix4D<T> transpose(Matrix4D<T> m) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i < j) {
					float temp = m.value[i][j];
					m.value[i][j] = m.value[j][i];
					m.value[j][i] = temp;
				}
			}
		}
		return m;
	}
	template <typename T>
	Matrix4D<T> inverse(Matrix4D<T> m) {
		m = transpose(m);
		Matrix4D<T> res;
		res.value[0][0] = m.value[1][1] * m.value[2][2] * m.value[3][3] + m.value[1][2] * m.value[2][3] * m.value[3][1] + m.value[1][3] * m.value[2][1] * m.value[3][2] - m.value[1][1] * m.value[2][3] * m.value[3][2] - m.value[1][2] * m.value[2][1] * m.value[3][3] - m.value[1][3] * m.value[2][2] * m.value[3][1];
		res.value[0][1] = m.value[0][1] * m.value[2][3] * m.value[3][2] + m.value[0][2] * m.value[2][1] * m.value[3][3] + m.value[0][3] * m.value[2][2] * m.value[3][1] - m.value[0][1] * m.value[2][2] * m.value[3][3] - m.value[0][2] * m.value[2][3] * m.value[3][1] - m.value[0][3] * m.value[2][1] * m.value[3][2];
		res.value[0][2] = m.value[0][1] * m.value[1][2] * m.value[3][3] + m.value[0][2] * m.value[1][3] * m.value[3][1] + m.value[0][3] * m.value[1][1] * m.value[3][2] - m.value[0][1] * m.value[1][3] * m.value[3][2] - m.value[0][2] * m.value[1][1] * m.value[3][3] - m.value[0][3] * m.value[1][2] * m.value[3][1];
		res.value[0][3] = m.value[0][1] * m.value[1][3] * m.value[2][2] + m.value[0][2] * m.value[1][1] * m.value[2][3] + m.value[0][3] * m.value[1][2] * m.value[2][1] - m.value[0][1] * m.value[1][2] * m.value[2][3] - m.value[0][2] * m.value[1][3] * m.value[2][1] - m.value[0][3] * m.value[1][1] * m.value[2][2];
		res.value[1][0] = m.value[1][0] * m.value[2][3] * m.value[3][2] + m.value[1][2] * m.value[2][0] * m.value[3][3] + m.value[1][3] * m.value[2][2] * m.value[3][0] - m.value[1][0] * m.value[2][2] * m.value[3][3] - m.value[1][2] * m.value[2][3] * m.value[3][0] - m.value[1][3] * m.value[2][0] * m.value[3][2];
		res.value[1][1] = m.value[0][0] * m.value[2][2] * m.value[3][3] + m.value[0][2] * m.value[2][3] * m.value[3][0] + m.value[0][3] * m.value[2][0] * m.value[3][2] - m.value[0][0] * m.value[2][3] * m.value[3][2] - m.value[0][2] * m.value[2][0] * m.value[3][3] - m.value[0][3] * m.value[2][2] * m.value[3][0];
		res.value[1][2] = m.value[0][0] * m.value[1][3] * m.value[3][2] + m.value[0][2] * m.value[1][0] * m.value[3][3] + m.value[0][3] * m.value[1][2] * m.value[3][0] - m.value[0][0] * m.value[1][2] * m.value[3][3] - m.value[0][2] * m.value[1][3] * m.value[3][0] - m.value[0][3] * m.value[1][0] * m.value[3][2];
		res.value[1][3] = m.value[0][0] * m.value[1][2] * m.value[2][3] + m.value[0][2] * m.value[1][3] * m.value[2][0] + m.value[0][3] * m.value[1][0] * m.value[2][2] - m.value[0][0] * m.value[1][3] * m.value[2][2] - m.value[0][2] * m.value[1][0] * m.value[2][3] - m.value[0][3] * m.value[1][2] * m.value[2][0];
		res.value[2][0] = m.value[1][0] * m.value[2][1] * m.value[3][3] + m.value[1][1] * m.value[2][3] * m.value[3][0] + m.value[1][3] * m.value[2][0] * m.value[3][1] - m.value[1][0] * m.value[2][3] * m.value[3][1] - m.value[1][1] * m.value[2][0] * m.value[3][3] - m.value[1][3] * m.value[2][1] * m.value[3][0];
		res.value[2][1] = m.value[0][0] * m.value[2][3] * m.value[3][1] + m.value[0][1] * m.value[2][0] * m.value[3][3] + m.value[0][3] * m.value[2][1] * m.value[3][0] - m.value[0][0] * m.value[2][1] * m.value[3][3] - m.value[0][1] * m.value[2][3] * m.value[3][0] - m.value[0][3] * m.value[2][0] * m.value[3][1];
		res.value[2][2] = m.value[0][0] * m.value[1][1] * m.value[3][3] + m.value[0][1] * m.value[1][3] * m.value[3][0] + m.value[0][3] * m.value[1][0] * m.value[3][1] - m.value[0][0] * m.value[1][3] * m.value[3][1] - m.value[0][1] * m.value[1][0] * m.value[3][3] - m.value[0][3] * m.value[1][1] * m.value[3][0];
		res.value[2][3] = m.value[0][0] * m.value[1][3] * m.value[2][1] + m.value[0][1] * m.value[1][0] * m.value[2][3] + m.value[0][3] * m.value[1][1] * m.value[2][0] - m.value[0][0] * m.value[1][1] * m.value[2][3] - m.value[0][1] * m.value[1][3] * m.value[2][0] - m.value[0][3] * m.value[1][0] * m.value[2][1];
		res.value[3][0] = m.value[1][0] * m.value[2][2] * m.value[3][1] + m.value[1][1] * m.value[2][0] * m.value[3][2] + m.value[1][2] * m.value[2][1] * m.value[3][0] - m.value[1][0] * m.value[2][1] * m.value[3][2] - m.value[1][1] * m.value[2][2] * m.value[3][0] - m.value[1][2] * m.value[2][0] * m.value[3][1];
		res.value[3][1] = m.value[0][0] * m.value[2][1] * m.value[3][2] + m.value[0][1] * m.value[2][2] * m.value[3][0] + m.value[0][2] * m.value[2][0] * m.value[3][1] - m.value[0][0] * m.value[2][2] * m.value[3][1] - m.value[0][1] * m.value[2][0] * m.value[3][2] - m.value[0][2] * m.value[2][1] * m.value[3][0];
		res.value[3][2] = m.value[0][0] * m.value[1][2] * m.value[3][1] + m.value[0][1] * m.value[1][0] * m.value[3][2] + m.value[0][2] * m.value[1][1] * m.value[3][0] - m.value[0][0] * m.value[1][1] * m.value[3][2] - m.value[0][1] * m.value[1][2] * m.value[3][0] - m.value[0][2] * m.value[1][0] * m.value[3][1];
		res.value[3][3] = m.value[0][0] * m.value[1][1] * m.value[2][2] + m.value[0][1] * m.value[1][2] * m.value[2][0] + m.value[0][2] * m.value[1][0] * m.value[2][1] - m.value[0][0] * m.value[1][2] * m.value[2][1] - m.value[0][1] * m.value[1][0] * m.value[2][2] - m.value[0][2] * m.value[1][1] * m.value[2][0];

		float det = m.value[0][0] * (m.value[1][1] * m.value[2][2] * m.value[3][3] - m.value[1][1] * m.value[2][3] * m.value[3][2] - m.value[1][2] * m.value[2][1] * m.value[3][3] + m.value[1][2] * m.value[2][3] * m.value[3][1] + m.value[1][3] * m.value[2][1] * m.value[3][2] - m.value[1][3] * m.value[2][2] * m.value[3][1]) +
			m.value[0][1] * (m.value[1][0] * m.value[2][2] * m.value[3][3] - m.value[1][0] * m.value[2][3] * m.value[3][2] - m.value[1][2] * m.value[2][0] * m.value[3][3] + m.value[1][2] * m.value[2][3] * m.value[3][0] + m.value[1][3] * m.value[2][1] * m.value[3][2] - m.value[1][3] * m.value[2][2] * m.value[3][0]) +
			m.value[0][2] * (m.value[1][0] * m.value[2][1] * m.value[3][3] - m.value[1][0] * m.value[2][3] * m.value[3][1] - m.value[1][1] * m.value[2][0] * m.value[3][3] + m.value[1][1] * m.value[2][3] * m.value[3][0] + m.value[1][3] * m.value[2][0] * m.value[3][1] - m.value[1][3] * m.value[2][1] * m.value[3][0]) +
			m.value[0][3] * (m.value[1][0] * m.value[2][1] * m.value[3][2] - m.value[1][0] * m.value[2][2] * m.value[3][1] - m.value[1][1] * m.value[2][0] * m.value[3][2] + m.value[1][1] * m.value[2][2] * m.value[3][0] + m.value[1][2] * m.value[2][0] * m.value[3][1] - m.value[1][2] * m.value[2][1] * m.value[3][0]);

		if (det == 0) {
			return NULL;
		}
		else if (det < 0) {
			det = det * (-1.0f);
		}
		res = res * (1.0f / det);

		res = transpose(res);
		return res;
	}

	template <typename T>
	Matrix4D<T> translate(Vec3<T> vec) {
		Matrix4D<T> mat;
		mat.value[0][3] = vec.x;
		mat.value[1][3] = vec.y;
		mat.value[2][3] = vec.z;
		return mat;
	}
	template <typename T>
	Matrix4D<T> scale(float scalar) {
		Matrix4D<T> mat;
		mat.value[0][0] *= scalar;
		mat.value[1][1] *= scalar;
		mat.value[2][2] *= scalar;
		return mat;
	}
	template <typename T>
	Matrix4D<T> scale(Vec3<T> vec) {
		Matrix4D<T> mat;
		mat.value[0][0] *= vec.x;
		mat.value[1][1] *= vec.y;
		mat.value[2][2] *= vec.z;
		return mat;
	}
	template <typename T>
	Matrix4D<T> rotate(float deg, Vec3<T> vec) {
		Matrix4D<T> mat;
		if (length(vec) == 0) {
			return mat;
		}
		float rad = deg * (PI / 180);
		vec = normalize(vec);
		mat.value[0][0] = vec.x * vec.x * (1 - std::cos(rad)) + std::cos(rad);
		mat.value[0][1] = vec.x * vec.y * (1 - std::cos(rad)) - vec.z * sin(rad);
		mat.value[0][2] = vec.x * vec.z * (1 - std::cos(rad)) + vec.y * sin(rad);

		mat.value[1][0] = vec.y * vec.x * (1 - std::cos(rad)) + vec.z * sin(rad);
		mat.value[1][1] = vec.y * vec.y * (1 - std::cos(rad)) + std::cos(rad);
		mat.value[1][2] = vec.y * vec.z * (1 - std::cos(rad)) - vec.x * sin(rad);

		mat.value[2][0] = vec.z * vec.x * (1 - std::cos(rad)) - vec.y * sin(rad);
		mat.value[2][1] = vec.z * vec.y * (1 - std::cos(rad)) + vec.x * sin(rad);
		mat.value[2][2] = vec.z * vec.z * (1 - std::cos(rad)) + std::cos(rad);
		return mat;
	}
	template <typename T>
	Matrix4D<T> rotate(Quaternion<T> q) {
		Matrix4D<T> out;
		q = normalize(q);
		out.value[0][0] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
		out.value[0][1] = 2 * q.x*q.y - 2 * q.z*q.w;
		out.value[0][2] = 2 * q.x*q.z + 2 * q.y*q.w;

		out.value[1][0] = 2 * q.x*q.y + 2 * q.z*q.w;
		out.value[1][1] = 1 - 2 * q.x*q.x - 2 * q.z*q.z;
		out.value[1][2] = 2 * q.y*q.z - 2 * q.x*q.w;

		out.value[2][0] = 2 * q.x*q.z - 2 * q.y*q.w;
		out.value[2][1] = 2 * q.y*q.z + 2 * q.x*q.w;
		out.value[2][2] = 1 - 2 * q.x*q.x - 2 * q.y*q.y;
		return out;
	}

	template <typename T>
	Matrix4D<T> perspective(float fov, float aspect, float near, float far) {
		Matrix4D<T> mat;
		float top = near * tan((PI / 180) * (fov / 2));
		float bottom = 0 - top;
		float right = top * aspect;
		float left = 0 - right;
		mat.value[0][0] = (2 * near) / (right - left);
		mat.value[0][2] = (right + left) / (right - left);
		mat.value[1][1] = (2 * near) / (top - bottom);
		mat.value[1][2] = (top + bottom) / (top - bottom);
		mat.value[2][2] = 0 - ((far + near) / (far - near));
		mat.value[2][3] = 0 - ((2 * far * near) / (far - near));
		mat.value[3][2] = -1;
		mat.value[3][3] = 0;
		return mat;
	}
	template <typename T>
	Matrix4D<T> orthographic(float left, float right, float bottom, float top, float near, float far) {
		Matrix4D<T> mat;
		mat.value[0][0] = 2 / (right - left);
		mat.value[0][3] = -((right + left) / (right - left));
		mat.value[1][1] = 2 / (top - bottom);
		mat.value[1][3] = -((top + bottom) / (top - bottom));
		mat.value[2][2] = -2 / (far - near);
		mat.value[2][3] = -((far + near) / (far - near));
		mat.value[3][3] = 1;
		return mat;
	}
	template <typename T>
	Matrix4D<T> lookAt(Vec3<T> position, Vec3<T> target, Vec3<T> up) {
		Vec3<T> direction = position - target;
		Vec3<T> right = normalize(cross(up, direction));
		float arr[] = {
			right.x,     right.y,     right.z,     0,
			up.x,        up.y,        up.z,        0,
			direction.x, direction.y, direction.z, 0,
			0,           0,           0,           1
		};
		Matrix4D<T> mat1(arr);

		Matrix4D<T> mat2;
		mat2.value[0][3] = 0 - position.x;
		mat2.value[1][3] = 0 - position.y;
		mat2.value[2][3] = 0 - position.z;
		return mat1 * mat2;
	}
}