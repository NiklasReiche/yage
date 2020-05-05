#pragma once

namespace gml
{
	template <typename T, size_t Size>
	VectorBase<T, Size>::VectorBase()
		: data{}
	{
	}

	template <typename T, size_t Size>
	VectorBase<T, Size>::VectorBase(T value)
	{
		data.fill(value);
	}

	template <typename T, size_t Size>
	VectorBase<T, Size>::VectorBase(const std::initializer_list<T>& init)
	{
		if (init.size() != Size)
		{
			throw InvalidDimensionException();
		}
		std::copy(std::begin(init), std::end(init), std::begin(data));
	}

	template <typename T, size_t Size>
	template <typename T2>
	VectorBase<T, Size>::VectorBase(const VectorBase<T2, Size>& other)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			this->at(i) = other.at(i);
		}
	}

	template <typename T, size_t Size>
	template <typename T2>
	VectorBase<T, Size>& VectorBase<T, Size>::operator=(const VectorBase<T2, Size>& other)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			this->at(i) = other.at(i);
		}
		return *this;
	}

	template <typename T, size_t Size>
	T& VectorBase<T, Size>::at(size_t pos)
	{
		return data.at(pos);
	}

	template <typename T, size_t Size>
	const T& VectorBase<T, Size>::at(size_t pos) const
	{
		return data.at(pos);
	}

	template <typename T, size_t Size>
	double VectorBase<T, Size>::sqrLength() const
	{
		return dot(*this, *this);
	}

	template <typename T, size_t Size>
	double VectorBase<T, Size>::length() const
	{
		return std::sqrt(sqrLength());
	}

	template <typename T, size_t Size>
	VectorBase<T, Size>& VectorBase<T, Size>::normalize()
	{
		const auto len = length();
		if (len == 0)
		{
			throw DivideByZeroException();
		}
		return operator*=(static_cast<T>(1 / len));
	}

	template <typename T, size_t Size>
	VectorBase<T, Size> VectorBase<T, Size>::operator-() const
	{
		VectorBase<T, Size> result;
		for (size_t i = 0; i < Size; ++i)
		{
			result.at(i) = -this->at(i);
		}
		return result;
	}

	template <typename T, size_t Size>
	VectorBase<T, Size>& VectorBase<T, Size>::operator+=(const VectorBase<T, Size>& right)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			this->at(i) += right.at(i);
		}
		return *this;
	}

	template <typename T, size_t Size>
	VectorBase<T, Size>& VectorBase<T, Size>::operator-=(const VectorBase<T, Size>& right)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			this->at(i) -= right.at(i);
		}
		return *this;
	}

	template <typename T, size_t Size>
	template <typename T2>
	VectorBase<T, Size>& VectorBase<T, Size>::operator*=(const T2& right)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			this->at(i) *= right;
		}
		return *this;
	}

	template <typename T, size_t Size>
	template <typename T2>
	VectorBase<T, Size>& VectorBase<T, Size>::operator/=(const T2& right)
	{
		if (right == 0)
		{
			throw DivideByZeroException();
		}

		for (size_t i = 0; i < Size; ++i)
		{
			this->at(i) /= right;
		}
		return *this;
	}

	template<typename T, size_t Size>
	VectorBase <T, Size> normalize(VectorBase <T, Size> vector)
	{
		return vector.normalize();
	}

	template<typename T, size_t Size>
	T dot(const VectorBase <T, Size> &left, const VectorBase <T, Size> &right)
	{
		T result = 0;
		for (size_t i = 0; i < Size; ++i)
		{
			result += left.at(i) * right.at(i);
		}
		return result;
	}

	template<typename T, size_t Size>
	double angle(const VectorBase <T, Size> &left, const VectorBase <T, Size> &right)
	{
		const double lengths = left.length() * right.length();
		if (lengths == 0.0)
		{
			throw DivideByZeroException();
		}
		return std::acos(dot(left, right) / lengths);
	}

	template <typename T, size_t Size>
	std::ostream& operator<<(std::ostream& os, const VectorBase<T, Size>& vec)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			os << vec.at(i) << " ";
		}
		return os;
	}

	template <typename T, size_t Size>
	bool operator==(const VectorBase<T, Size>& left, const VectorBase<T, Size>& right)
	{
		for (size_t i = 0; i < Size; ++i)
		{
			if (left.at(i) != right.at(i))
				return false;
		}
		return true;
	}

	template <typename T, size_t Size>
	bool operator!=(const VectorBase<T, Size>& left, const VectorBase<T, Size>& right)
	{
		return !(left == right);
	}

	template <typename T, size_t Size>
	VectorBase<T, Size> operator+(const VectorBase<T, Size>& left, const VectorBase<T, Size>& right)
	{
		return VectorBase<T, Size>(left) += right;
	}

	template <typename T, size_t Size>
	VectorBase<T, Size> operator-(const VectorBase<T, Size>& left, const VectorBase<T, Size>& right)
	{
		return VectorBase<T, Size>(left) -= right;
	}

	template <typename T, size_t Size>
	VectorBase<T, Size> operator*(const double left, const VectorBase<T, Size>& right)
	{
		return VectorBase<T, Size>(right) *= left;
	}

	template <typename T, size_t Size>
	VectorBase<T, Size> operator*(const VectorBase<T, Size>& left, const double right)
	{
		return VectorBase<T, Size>(left) *= right;
	}

	template <typename T, size_t Size>
	VectorBase<T, Size> operator/(const VectorBase<T, Size>& left, const double right)
	{
		return VectorBase<T, Size>(left) /= right;
	}
}