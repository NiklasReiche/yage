#pragma once

namespace gml_test
{
	class ComplexD
	{
	public:
		double real;
		double imaginary;

		ComplexD()
			: real(0), imaginary(0)
		{
		}

		ComplexD(double a, double b)
			: real(a), imaginary(b)
		{
		}
	};

	class ComplexF
	{
	public:
		float real;
		float imaginary;

		ComplexF()
			: real(0), imaginary(0)
		{
		}

		ComplexF(const ComplexD& other)
			: real(static_cast<float>(other.real)),
			  imaginary(static_cast<float>(other.imaginary))
		{
		}

		ComplexF& operator=(ComplexD& other)
		{
			this->real = static_cast<float>(other.real);
			this->imaginary = static_cast<float>(other.imaginary);
			return *this;
		}
	};
}