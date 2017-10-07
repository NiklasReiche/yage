#pragma once
#include <iostream>
#include <cmath>

const float PI = 3.141592653589793238463f;

namespace gml
{
	/*******************************
	Forward Declarations
	*******************************/

	template <typename T>
	class Vector2D;
	template <typename T>
	class Vector3D;
	template <typename T>
	class Vector4D;
	template <typename T>
	class Matrix4D;
	template <typename T>
	class Quaternion;


	template <typename T>
	using Vec2 = Vector2D<T>;
	template <typename T>
	using Vec3 = Vector3D<T>;
	template <typename T>
	using Vec4 = Vector4D<T>;


	typedef Vec2<int> Vec2i;
	typedef Vec3<int> Vec3i;
	typedef Vec4<int> Vec4i;

	typedef Vec2<unsigned int> Vec2ui;
	typedef Vec3<unsigned int> Vec3ui;
	typedef Vec4<unsigned int> Vec4ui;

	typedef Vec2<float> Vec2f;
	typedef Vec3<float> Vec3f;
	typedef Vec4<float> Vec4f;

	typedef Vec2<double> Vec2d;
	typedef Vec3<double> Vec3d;
	typedef Vec4<double> Vec4d;


	/*******************************
	Classes
	*******************************/

	template <typename T>
	class Vector2D
	{
	public:
		T x, y;

		Vector2D() :
			x(0), y(0) {}
		Vector2D(T s) :
			x(s), y(s) {}
		Vector2D(T x, T y) :
			x(x), y(y) {}

		Vector2D& operator+=(const Vector2D& right)
		{
			this->x += right.x;
			this->y += right.y;
			return *this;
		}
		Vector2D& operator-=(const Vector2D& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			return *this;
		}
		Vector2D& operator*=(float right)
		{
			this->x *= right;
			this->y *= right;
			return *this;
		}
		Vector2D& operator/=(float right)
		{
			this->x /= right;
			this->y /= right;
			return *this;
		}
	};

	template <typename T>
	class Vector3D
	{
	public:
		T x, y, z;

		Vector3D() :
			x(0), y(0), z(0) {}
		Vector3D(T s) :
			x(s), y(s), z(s) {}
		Vector3D(T x, T y, T z) :
			x(x), y(y), z(z) {}

		Vector3D& operator+=(const Vector3D& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			return *this;
		}
		Vector3D& operator-=(const Vector3D& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			return *this;
		}
		Vector3D& operator*=(float right)
		{
			this->x *= right;
			this->y *= right;
			this->z *= right;
			return *this;
		}
		Vector3D& operator/=(float right)
		{
			this->x /= right;
			this->y /= right;
			this->z /= right;
			return *this;
		}
	};

	template <typename T>
	class Vector4D
	{
	public:
		T x, y, z, w;

		Vector4D() :
			x(0), y(0), z(0), w(0) {}
		Vector4D(T s) :
			x(s), y(s), z(s), w(s) {}
		Vector4D(T x, T y, T z, T w) :
			x(x), y(y), z(z), w(w) {}

		Vector3D<T> toVec3D()
		{
			Vector3D<T> vec;
			vec.x = this->x * this->w;
			vec.y = this->y * this->w;
			vec.z = this->z * this->w;
			return vec;
		}

		Vector4D& operator+=(const Vector4D& right)
		{
			this->x += right.x;
			this->y += right.y;
			this->z += right.z;
			this->w += right.w;
			return *this;
		}
		Vector4D& operator-=(const Vector4D& right)
		{
			this->x -= right.x;
			this->y -= right.y;
			this->z -= right.z;
			this->w -= right.w;
			return *this;
		}
		Vector4D& operator*=(float right)
		{
			this->x *= right;
			this->y *= right;
			this->z *= right;
			this->w *= right;
			return *this;
		}
		Vector4D& operator/=(float right)
		{
			this->x /= right;
			this->y /= right;
			this->z /= right;
			this->w /= right;
			return *this;
		}
	};

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
	class Quaternion
	{
	public:
		T w, x, y, z;

		Quaternion() :
			w(0), x(0), y(0), z(0) {}
		Quaternion(T w, T x, T y, T z) :
			w(w), x(x), y(y), z(z) {}
		Quaternion(Vector3D<T> vec) :
			w(0), x(vec.x), y(vec.y), z(vec.z) {}

		Vector3D<T> getForward()
		{
			return normalize(Vector3D<T>(-2 * (x * z + w * y), -2 * (y * z - w * x), -1 + 2 * (x * x + y * y)));
		}
		Vector3D<T> getRight()
		{
			return normalize(Vector3D<T>(1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)));
		}
		Vector3D<T> getUp()
		{
			return normalize(Vector3D<T>(2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)));
		}
	};


	/*******************************
	Operator Overloading
	*******************************/

	/*  Output Stream */
	template <typename T>
	std::ostream& operator<<(std::ostream& os, Vector2D<T> vec) {
		os << vec.x << " " << vec.y;
		return os;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, Vector3D<T> vec) {
		os << vec.x << " " << vec.y << " " << vec.z;
		return os;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, Vector4D<T> vec) {
		os << vec.x << " " << vec.y << " " << vec.z << " " << vec.w;
		return os;
	}
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
	std::ostream& operator<<(std::ostream& os, Quaternion<T> vec) {
		os << vec.w << " " << vec.x << " " << vec.y << " " << vec.z;
		return os;
	}

	/*  Comparision */
	template <typename T>
	bool operator==(const Vector2D<T>& left, const Vector2D<T>& right) {
		return left.x == right.x && left.y == right.y;
	}
	template <typename T>
	bool operator==(const Vector3D<T>& left, const Vector3D<T>& right) {
		return left.x == right.x && left.y == right.y && left.z == right.z;
	}
	template <typename T>
	bool operator==(const Vector4D<T>& left, const Vector4D<T>& right) {
		return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
	}

	template <typename T>
	bool operator!=(const Vector2D<T>& left, const Vector2D<T>& right) {
		return !(left == right);
	}
	template <typename T>
	bool operator!=(const Vector3D<T>& left, const Vector3D<T>& right) {
		return !(left == right);
	}
	template <typename T>
	bool operator!=(const Vector4D<T>& left, const Vector4D<T>& right) {
		return !(left == right);
	}

	/*  Addition */
	template <typename T>
	Vector2D<T> operator+(Vector2D<T> left, Vector2D<T> right) {
		Vector2D<T> out;
		out.x = left.x + right.x;
		out.y = left.y + right.y;
		return out;
	}
	template <typename T>
	Vector3D<T> operator+(Vector3D<T> vec1, Vector3D<T> vec2) {
		Vector3D<T> vec3;
		vec3.x = vec1.x + vec2.x;
		vec3.y = vec1.y + vec2.y;
		vec3.z = vec1.z + vec2.z;
		return vec3;
	}
	template <typename T>
	Vector4D<T> operator+(Vector4D<T> vec1, Vector4D<T> vec2) {
		Vector4D<T> vec3;
		vec3.x = vec1.x + vec2.x;
		vec3.y = vec1.y + vec2.y;
		vec3.z = vec1.z + vec2.z;
		vec3.w = vec1.w + vec2.w;
		return vec3;
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

	/*  Subtraction */
	template <typename T>
	Vector2D<T> operator-(Vector2D<T> vec1, Vector2D<T> vec2) {
		Vector2D<T> vec3;
		vec3.x = vec1.x - vec2.x;
		vec3.y = vec1.y - vec2.y;
		return vec3;
	}
	template <typename T>
	Vector3D<T> operator-(Vector3D<T> vec1, Vector3D<T> vec2) {
		Vector3D<T> vec3;
		vec3.x = vec1.x - vec2.x;
		vec3.y = vec1.y - vec2.y;
		vec3.z = vec1.z - vec2.z;
		return vec3;
	}
	template <typename T>
	Vector4D<T> operator-(Vector4D<T> vec1, Vector4D<T> vec2) {
		Vector4D<T> vec3;
		vec3.x = vec1.x - vec2.x;
		vec3.y = vec1.y - vec2.y;
		vec3.z = vec1.z - vec2.z;
		vec3.w = vec1.w - vec2.w;
		return vec3;
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

	/*  Multiplication */
	template <typename T>
	Vector2D<T> operator*(Vector2D<T> left, Vector2D<T> right) {
		Vector2D<T> out;
		out.x = left.x * right.x;
		out.y = left.y * right.y;
		return out;
	}
	template <typename T>
	Vector2D<T> operator*(Vector2D<T> vec, float s) {
		Vector2D<T> vec2;
		vec2.x = vec.x * s;
		vec2.y = vec.y * s;
		return vec2;
	}
	template <typename T>
	Vector2D<T> operator*(float s, Vector2D<T> vec) {
		Vector2D<T> vec2;
		vec2.x = s * vec.x;
		vec2.y = s * vec.y;
		return vec2;
	}

	template <typename T>
	Vector3D<T> operator*(Vector3D<T> left, Vector3D<T> right) {
		Vector3D<T> out;
		out.x = left.x * right.x;
		out.y = left.y * right.y;
		out.z = left.z * right.z;
		return out;
	}
	template <typename T>
	Vector3D<T> operator*(Vector3D<T> vec, float s) {
		Vector3D<T> vec2;
		vec2.x = vec.x * s;
		vec2.y = vec.y * s;
		vec2.z = vec.z * s;
		return vec2;
	}
	template <typename T>
	Vector3D<T> operator*(float s, Vector3D<T> vec) {
		Vector3D<T> vec2;
		vec2.x = s * vec.x;
		vec2.y = s * vec.y;
		vec2.z = s * vec.z;
		return vec2;
	}

	template <typename T>
	Vector4D<T> operator*(Vector4D<T> left, Vector4D<T> right) {
		Vector4D<T> out;
		out.x = left.x * right.x;
		out.y = left.y * right.y;
		out.z = left.z * right.z;
		out.w = left.w * right.w;
		return out;
	}
	template <typename T>
	Vector4D<T> operator*(Vector4D<T> vec, float s) {
		Vector4D<T> out;
		out.x = vec.x * s;
		out.y = vec.y * s;
		out.z = vec.z * s;
		out.w = vec.w * s;
		return out;
	}
	template <typename T>
	Vector4D<T> operator*(float s, Vector4D<T> vec) {
		Vector4D<T> out;
		out.x = s * vec.x;
		out.y = s * vec.y;
		out.z = s * vec.z;
		out.w = s * vec.w;
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
	Vector3D<T> operator*(Matrix4D<T> mat, Vector3D<T> vec1) {
		Vector3D<T> vec2;
		vec2.x = mat.value[0][0] * vec1.x + mat.value[0][1] * vec1.y + mat.value[0][2] * vec1.z;
		vec2.y = mat.value[1][0] * vec1.x + mat.value[1][1] * vec1.y + mat.value[1][2] * vec1.z;
		vec2.z = mat.value[2][0] * vec1.x + mat.value[2][1] * vec1.y + mat.value[2][2] * vec1.z;
		return vec2;
	}
	template <typename T>
	Vector4D<T> operator*(Matrix4D<T> mat, Vector4D<T> vec1) {
		Vector4D<T> vec2;
		vec2.x = mat.value[0][0] * vec1.x + mat.value[0][1] * vec1.y + mat.value[0][2] * vec1.z + mat.value[0][3] * vec1.z;
		vec2.y = mat.value[1][0] * vec1.x + mat.value[1][1] * vec1.y + mat.value[1][2] * vec1.z + mat.value[1][3] * vec1.z;
		vec2.z = mat.value[2][0] * vec1.x + mat.value[2][1] * vec1.y + mat.value[2][2] * vec1.z + mat.value[2][3] * vec1.z;
		vec2.w = mat.value[3][0] * vec1.x + mat.value[3][1] * vec1.y + mat.value[3][2] * vec1.z + mat.value[3][3] * vec1.z;
		return vec2;
	}

	/*  Division */
	template <typename T>
	Vector2D<T> operator/(Vector2D<T> vec, float s) {
		Vector2D<T> out;
		out.x = vec.x / s;
		out.y = vec.y / s;
		return out;
	}
	template <typename T>
	Vector3D<T> operator/(Vector3D<T> vec, float s) {
		Vector3D<T> out;
		out.x = vec.x / s;
		out.y = vec.y / s;
		out.z = vec.z / s;
		return out;
	}
	template <typename T>
	Vector4D<T> operator/(Vector4D<T> vec, float s) {
		Vector4D<T> out;
		out.x = vec.x / s;
		out.y = vec.y / s;
		out.z = vec.z / s;
		out.w = vec.w / s;
		return out;
	}
	template <typename T>
	Quaternion<T> operator/(Quaternion<T> quat, float s) {
		Quaternion<T> out;
		out.x = quat.x / s;
		out.y = quat.y / s;
		out.z = quat.z / s;
		out.w = quat.w / s;
		return out;
	}


	/*  Vector Calculations */
	template <typename T>
	Vector3D<T> cross(Vector3D<T> vec1, Vector3D<T> vec2) {
		Vector3D<T> vec3;
		vec3.x = vec1.y * vec2.z - vec1.z * vec2.y;
		vec3.y = vec1.z * vec2.x - vec1.x * vec2.z;
		vec3.z = vec1.x * vec2.y - vec1.y * vec2.x;
		return vec3;
	}
	template <typename T>
	T dot(Vector3D<T> vec1, Vector3D<T> vec2) {
		return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	}

	template <typename T>
	double length(Vector2D<T> vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}
	template <typename T>
	double length(Vector3D<T> vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}
	template <typename T>
	double length(Vector4D<T> vec) {
		return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
	}

	template <typename T>
	Vector2D<T> normalize(Vector2D<T> vec) {
		if (length(vec) == 0) {
			return vec;
		}
		return (1 / length(vec)) * vec;
	}
	template <typename T>
	Vector3D<T> normalize(Vector3D<T> vec) {
		if (length(vec) == 0) {
			return vec;
		}
		return (1 / length(vec)) * vec;
	}
	template <typename T>
	Vector4D<T> normalize(Vector4D<T> vec) {
		if (length(vec) == 0) {
			return vec;
		}
		return (1 / length(vec)) * vec;
	}

	template <typename T>
	Vector3D<T> copy(Vector3D<T> vec) {
		return Vector3D<T>(vec.x, vec.y, vec.z);
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
	Matrix4D<T> translate(Vector3D<T> vec) {
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
	Matrix4D<T> scale(Vector3D<T> vec) {
		Matrix4D<T> mat;
		mat.value[0][0] *= vec.x;
		mat.value[1][1] *= vec.y;
		mat.value[2][2] *= vec.z;
		return mat;
	}
	template <typename T>
	Matrix4D<T> rotate(float deg, Vector3D<T> vec) {
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
	Matrix4D<T> lookAt(Vector3D<T> position, Vector3D<T> target, Vector3D<T> up) {
		Vector3D<T> direction = position - target;
		Vector3D<T> right = normalize(cross(up, direction));
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


	/*  Quaternion Calculations */
	template <typename T>
	Quaternion<T> operator*(Quaternion<T> A, Quaternion<T> B) {
		Quaternion<T> C;
		Vector3D<T> a = Vector3D<T>(A.x, A.y, A.z);
		Vector3D<T> b = Vector3D<T>(B.x, B.y, B.z);
		C.w = A.w * B.w - dot(a, b);
		Vector3D<T> c = Vector3D<T>(A.w * b) + Vector3D<T>(B.w * a) + cross(a, b);
		C.x = c.x;
		C.y = c.y;
		C.z = c.z;
		/*
		C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
		C.y = A.w*B.y + A.y*B.w - A.x*B.z + A.z*B.x;
		C.z = A.w*B.z + A.z*B.w + A.x*B.y - A.y*B.x;
		C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
		*/
		return C;
	}
	template <typename T>
	Quaternion<T> operator*(Quaternion<T> quat, float s) {
		Quaternion<T> out;
		out.x = quat.x * s;
		out.y = quat.y * s;
		out.z = quat.z * s;
		out.w = quat.w * s;
		return out;
	}
	template <typename T>
	Quaternion<T> operator*(float s, Quaternion<T> quat) {
		Quaternion<T> out;
		out.x = s * quat.x;
		out.y = s * quat.y;
		out.z = s * quat.z;
		out.w = s * quat.w;
		return out;
	}

	template <typename T>
	float length(Quaternion<T> quat) {
		return sqrt(quat.x * quat.x + quat.y * quat.y + quat.z * quat.z + quat.w * quat.w);
	}
	template <typename T>
	Quaternion<T> normalize(Quaternion<T> quat) {
		if (length(quat) == 0) {
			return quat;
		}
		return (1 / length(quat)) * quat;
	}
	template <typename T>
	Quaternion<T> conjugate(Quaternion<T> quat) {
		Quaternion<T> out;
		out.w = quat.w;
		out.x = -quat.x;
		out.y = -quat.y;
		out.z = -quat.z;
		return out;
	}

	template <typename T>
	Quaternion<T> axisAngle(float angle, Vector3D<T> axis) {
		Quaternion<T> q;
		float rad = angle * (PI / 180);
		float s = sin(rad / 2);
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		q.w = std::cos(rad / 2);
		return q;
	}
	template <typename T>
	Quaternion<T> eulerAngle(float yaw, float pitch, float roll) {
		//Quaternion qroll = Quaternion(std::cos(angle_y / 2), sin(angle_y / 2), 0, 0);
		//Quaternion qpitch = Quaternion(std::cos(angle_x / 2), 0, sin(angle_x / 2), 0);
		//Quaternion qyaw = Quaternion(std::cos(angle_z / 2), 0, 0, sin(angle_z / 2));
		return Quaternion<T>();
	}
}