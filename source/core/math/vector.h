#pragma once

#include "vectorbase.h"

namespace gml
{
	template <typename T>
	class Vec1;
	template <typename T>
	class Vec2;
	template <typename T>
	class Vec3;
	template <typename T>
	class Vec4;

	template <typename T, size_t Size>
	using Vector = VectorBase<T, Size>;

	typedef Vec1<int> Vec1i;
	typedef Vec2<int> Vec2i;
	typedef Vec3<int> Vec3i;
	typedef Vec4<int> Vec4i;

	typedef Vec1<unsigned int> Vec1ui;
	typedef Vec2<unsigned int> Vec2ui;
	typedef Vec3<unsigned int> Vec3ui;
	typedef Vec4<unsigned int> Vec4ui;

	typedef Vec1<float> Vec1f;
	typedef Vec2<float> Vec2f;
	typedef Vec3<float> Vec3f;
	typedef Vec4<float> Vec4f;

	typedef Vec1<double> Vec1d;
	typedef Vec2<double> Vec2d;
	typedef Vec3<double> Vec3d;
	typedef Vec4<double> Vec4d;


	/**
	* @brief Represents a 3-dimensional generic vector.
	* THis class adds special member x for conveniance.
	*
	* @tparam T the type
	*/
	template<typename T>
	class Vec1 : public VectorBase<T, 1>
	{
	public:
		T & x = this->at(0);

		/** Inherit Constructors from VectorBase */
		using VectorBase<T, 1>::VectorBase;

		Vec1();

		explicit Vec1(T x);

		Vec1(const Vec1<T> & other);

		Vec1(const VectorBase<T, 1> & other);

		Vec1<T>& operator=(const Vec1<T> & other);

		Vec1<T>& operator=(const VectorBase<T, 1> & other);
	};

	/**
	* @brief Represents a 3-dimensional generic vector.
	* THis class adds special members x, y for conveniance.
	*
	* @tparam T the type
	*/
	template<typename T>
	class Vec2 : public VectorBase<T, 2>
	{
	public:
		T & x = this->at(0);
		T & y = this->at(1);

		/** Inherit Constructors from VectorBase */
		using VectorBase<T, 2>::VectorBase;

		Vec2();

		Vec2(T x, T y);

		Vec2(const Vec2<T> & other);

		Vec2(const VectorBase<T, 2> & other);

		Vec2<T>& operator=(const Vec2<T> & other);

		Vec2<T>& operator=(const VectorBase<T, 2> & other);
	};

	/**
	 * @brief Represents a 3-dimensional generic vector.
	 * THis class adds special members x, y, z for conveniance.
	 * 
	 * @tparam T the type
	 */
	template<typename T>
	class Vec3 : public VectorBase<T, 3>
	{
	public:
		T & x = this->at(0);
		T & y = this->at(1);
		T & z = this->at(2);

		/** Inherit Constructors from VectorBase */
		using VectorBase<T, 3>::VectorBase;

		Vec3();

		Vec3(T x, T y, T z);

		Vec3(const Vec3<T> & other);

		Vec3(const VectorBase<T, 3> & other);

		Vec3<T>& operator=(const Vec3<T> & other);

		Vec3<T>& operator=(const VectorBase<T, 3> & other);
	};

	/**
	* @brief Represents a 3-dimensional generic vector.
	* This class adds special members x, y, z, w for conveniance.
	*
	* @tparam T the type
	*/
	template<typename T>
	class Vec4 : public VectorBase<T, 4>
	{
	public:
		T & x = this->at(0);
		T & y = this->at(1);
		T & z = this->at(2);
		T & w = this->at(3);

		/** Inherit Constructors from VectorBase */
		using VectorBase<T, 4>::VectorBase;

		Vec4();

		Vec4(T x, T y, T z, T w);

		Vec4(const Vec4<T> & other);

		Vec4(const VectorBase<T, 4> & other);

		Vec4<T>& operator=(const Vec4<T> & other);

		Vec4<T>& operator=(const VectorBase<T, 4> & other);
	};


	/**
	 * @brief Calculates the cross product of two 3-dimensional vectors.
	 * 
	 * @tparam T the type
	 * @param left the first vector
	 * @param right the second vector
	 * @return the cross product
	 */
	template <typename T>
	Vec3<T> cross(Vec3<T> left, Vec3<T> right);

	/**
	 * @brief Calculates the angle bewtween two 2-dimensional vectors.
	 * 
	 * @tparam T the type
	 * @param left the first vector
	 * @param right the second vector
	 * @return the angle in radians
	 * 
	 * @throws DivideByZeroException if one or both vector's lengths is zero
	 */
	template <typename T>
	float angle(Vec2<T> left, Vec2<T> right);

	/**
	 * @brief Calculates the angle bewtween two 3-dimensional vectors.
	 * 
	 * @tparam T the type
	 * @param left the first vector
	 * @param right the second vector
	 * @return the angle in radians
	 * 
	 * @throws DivideByZeroException if one or both vector's lengths is zero
	 */
	template <typename T>
	float angle(Vec3<T> left, Vec3<T> right);
}

#include "vector.tpp"
