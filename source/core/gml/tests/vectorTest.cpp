#include <gtest/gtest.h>

#include <gml/maths.h>
#include <gml/vector.h>

using namespace gml;

class VectorTest : public testing::Test
{
public:
	class ComplexD
	{
	public:
		double real;
		double imaginary;

		ComplexD()
			: real(0), imaginary(0)
		{}

		ComplexD(double a, double b)
			: real(a), imaginary(b)
		{}
	};

	class ComplexF
	{
	public:
		float real;
		float imaginary;

		ComplexF()
			: real(0), imaginary(0)
		{}

		ComplexF(const ComplexD &other)
			: real(static_cast<float>(other.real)),
			  imaginary(static_cast<float>(other.imaginary))
		{}

		ComplexF &operator=(ComplexD &other)
		{
			this->real = static_cast<float>(other.real);
			this->imaginary = static_cast<float>(other.imaginary);
			return *this;
		}
	};
};

TEST(VectorTest, Constructor_Default_WithPrimitives)
{
	Vector<int, 5> vec;

	EXPECT_EQ(0, vec.at(0));
	EXPECT_EQ(0, vec.at(1));
	EXPECT_EQ(0, vec.at(2));
	EXPECT_EQ(0, vec.at(3));
	EXPECT_EQ(0, vec.at(4));
}

TEST(VectorTest, Constructor_Default_WithClasses)
{
	Vector<VectorTest::ComplexD, 2> complexVector;

	EXPECT_EQ(0, complexVector.at(0).real);
	EXPECT_EQ(0, complexVector.at(0).imaginary);
	EXPECT_EQ(0, complexVector.at(1).real);
	EXPECT_EQ(0, complexVector.at(1).imaginary);
}

TEST(VectorTest, Constructor_Fill)
{
	Vector<int, 3> vec(4);

	EXPECT_EQ(4, vec.at(0));
	EXPECT_EQ(4, vec.at(1));
	EXPECT_EQ(4, vec.at(2));
}

TEST(VectorTest, Constructor_InitializerList)
{
	Vector<float, 3> vec2{ 2.0f, -1.5f, 0.0f };
	EXPECT_EQ(2.0f, vec2.at(0));
	EXPECT_EQ(-1.5f, vec2.at(1));
	EXPECT_EQ(0.0f, vec2.at(2));
}

TEST(VectorTest, Constructor_InitializerList_InvalidDimension)
{
	EXPECT_THROW((Vector<int, 1>{ 0, 1 }), InvalidDimensionException);

	EXPECT_THROW((Vector<int, 3>{ 0, 1 }), InvalidDimensionException);
}

TEST(VectorTest, Constructor_Copy)
{
	Vector<int, 2> initial{ 2, 5 };

	Vector<int, 2> copy = initial;

	EXPECT_EQ(2, initial.at(0));
	EXPECT_EQ(5, initial.at(1));
	EXPECT_EQ(2, copy.at(0));
	EXPECT_EQ(5, copy.at(1));
}

TEST(VectorTest, Constructor_Conversion_WithPrimitive)
{
	Vector<int, 2> initial{ 2, 5 };

	Vector<float, 2> copy = initial;

	EXPECT_EQ(2, initial.at(0));
	EXPECT_EQ(5, initial.at(1));
	EXPECT_FLOAT_EQ(2, copy.at(0));
	EXPECT_FLOAT_EQ(5, copy.at(1));
}

TEST(VectorTest, Constructor_Conversion_WithClasses)
{
	Vector<VectorTest::ComplexD, 2> initial{{ 2, 5 },
	                                        { 1, 8 }};

	Vector<VectorTest::ComplexF, 2> copy = initial;

	EXPECT_DOUBLE_EQ(2, initial.at(0).real);
	EXPECT_DOUBLE_EQ(5, initial.at(0).imaginary);
	EXPECT_DOUBLE_EQ(1, initial.at(1).real);
	EXPECT_DOUBLE_EQ(8, initial.at(1).imaginary);

	EXPECT_FLOAT_EQ(2, copy.at(0).real);
	EXPECT_FLOAT_EQ(5, copy.at(0).imaginary);
	EXPECT_FLOAT_EQ(1, copy.at(1).real);
	EXPECT_FLOAT_EQ(8, copy.at(1).imaginary);
}

TEST(VectorTest, Assignment_Copy)
{
	Vector<int, 2> initial{ 2, 5 };
	Vector<int, 2> copy{ 0, 0 };

	copy = initial;

	EXPECT_EQ(2, initial.at(0));
	EXPECT_EQ(5, initial.at(1));
	EXPECT_EQ(2, copy.at(0));
	EXPECT_EQ(5, copy.at(1));
}

TEST(VectorTest, Assignment_Conversion_WithPrimitive)
{
	Vector<int, 2> initial{ 2, 5 };
	Vector<float, 2> copy;

	copy = initial;

	EXPECT_EQ(2, initial.at(0));
	EXPECT_EQ(5, initial.at(1));
	EXPECT_FLOAT_EQ(2, copy.at(0));
	EXPECT_FLOAT_EQ(5, copy.at(1));
}

TEST(VectorTest, Assignment_Conversion_WithClasses)
{
	Vector<VectorTest::ComplexD, 2> initial{{ 2, 5 },
	                                        { 1, 8 }};
	Vector<VectorTest::ComplexF, 2> copy;

	copy = initial;

	EXPECT_DOUBLE_EQ(2, initial.at(0).real);
	EXPECT_DOUBLE_EQ(5, initial.at(0).imaginary);
	EXPECT_DOUBLE_EQ(1, initial.at(1).real);
	EXPECT_DOUBLE_EQ(8, initial.at(1).imaginary);

	EXPECT_FLOAT_EQ(2, copy.at(0).real);
	EXPECT_FLOAT_EQ(5, copy.at(0).imaginary);
	EXPECT_FLOAT_EQ(1, copy.at(1).real);
	EXPECT_FLOAT_EQ(8, copy.at(1).imaginary);
}


TEST(VectorTest, Equality)
{
	ASSERT_TRUE(Vec3i({ 3, 2, 4 }) == Vec3i({ 3, 2, 4 }));
	ASSERT_FALSE(Vec3i({ 3, 1, 4 }) == Vec3i({ 3, 2, 4 }));
	ASSERT_TRUE(Vec3i({ 3, 1, 4 }) != Vec3i({ 3, 2, 4 }));
	ASSERT_FALSE(Vec3i({ 3, 2, 4 }) != Vec3i({ 3, 2, 4 }));
}

TEST(VectorTest, Add)
{
	const Vec3i vec1(2, 1, 3);
	const Vec3i vec2(2, -4, 0);

	Vec3i expected(4, -3, 3);
	Vec3i result;

	result = vec1 + vec2;
	EXPECT_EQ(expected, result);

	result = vec2 + vec1;
	EXPECT_EQ(expected, result);

	result = vec1;
	result += vec2;
	EXPECT_EQ(expected, result);
}

TEST(VectorTest, Subtract)
{
	const Vec3i vec1(2, 1, 3);
	const Vec3i vec2(2, -4, 0);

	Vec3i expected(0, 5, 3);
	Vec3i result;

	result = vec1 - vec2;
	EXPECT_EQ(expected, result);

	result = vec2 - vec1;
	expected = Vec3i(0, -5, -3);
	EXPECT_EQ(expected, result);

	result = vec2;
	result -= vec1;
	EXPECT_EQ(expected, result);

	EXPECT_EQ(Vec3i(-1, -3, 2), -Vec3i(1, 3, -2));
}

TEST(VectorTest, Multiply)
{
	const Vec3i vec1(2, 1, 3);
	const int factor = 3;

	Vec3i expected(6, 3, 9);
	Vec3i result;

	result = vec1 * factor;
	EXPECT_EQ(expected, result);

	result = factor * vec1;
	EXPECT_EQ(expected, result);

	result = vec1;
	result *= factor;
	EXPECT_EQ(expected, result);
}

TEST(VectorTest, Divide)
{
	const Vec3d vec1(2, 1, 3);
	const double factor = 2;

	Vec3d expected(1, 0.5, 1.5);
	Vec3d result;

	result = vec1 / factor;
	EXPECT_EQ(expected, result);

	result = vec1;
	result /= factor;
	EXPECT_EQ(expected, result);

	EXPECT_THROW((result /= 0), DivideByZeroException);
	EXPECT_THROW((result = result / 0.0), DivideByZeroException);
}


TEST(VectorTest, Dot)
{
	const Vec3d vec1(2, 1, 3);
	const Vec3d vec2(-2, 0, 1);

	double result = dot(vec1, vec2);

	EXPECT_EQ(-1, result);
}

TEST(VectorTest, Normalize)
{
	Vec3d vec1(2, 1, 3);

	Vec3d expected(2 / std::sqrt(14), 1 / std::sqrt(14), 3 / std::sqrt(14));

	EXPECT_DOUBLE_EQ(std::sqrt(14), vec1.length());

	Vec3d result = normalize(vec1);
	EXPECT_DOUBLE_EQ(expected.x, result.x);
	EXPECT_DOUBLE_EQ(expected.y, result.y);
	EXPECT_DOUBLE_EQ(expected.z, result.z);

	vec1.normalize();
	EXPECT_DOUBLE_EQ(expected.x, vec1.x);
	EXPECT_DOUBLE_EQ(expected.y, vec1.y);
	EXPECT_DOUBLE_EQ(expected.z, vec1.z);

	EXPECT_THROW((Vec2i(0, 0).normalize()), DivideByZeroException);
}

TEST(VectorTest, Length)
{
	Vec3d vec1(2, 1, 3);

	EXPECT_DOUBLE_EQ(14, vec1.sqrLength());
	EXPECT_DOUBLE_EQ(std::sqrt(14), vec1.length());
}

TEST(VectorTest, Cross)
{
	Vec3d vec1(2, 1, 3);
	Vec3d vec2(-1, 5, 1);

	Vec3d expected(-14, -5, 11);
	Vec3d result = cross(vec1, vec2);
	EXPECT_EQ(expected, result);
}

TEST(VectorTest, Angle)
{
	Vec3d vec1(1, 0, 0);
	Vec3d vec2(1, 1, 0);

	EXPECT_DOUBLE_EQ(toRad(45.0), angle(vec1, vec2));

	EXPECT_THROW((angle(Vec3d(), Vec3d())), DivideByZeroException);
	EXPECT_THROW((angle(vec1, Vec3d())), DivideByZeroException);
	EXPECT_THROW((angle(Vec3d(), vec2)), DivideByZeroException);


	Vec2d vec3(1, 0);
	Vec2d vec4(1, 1);

	EXPECT_DOUBLE_EQ(toRad(45.0), angle(vec3, vec4));

	EXPECT_THROW((angle(Vec2d(), Vec2d())), DivideByZeroException);
	EXPECT_THROW((angle(vec3, Vec2d())), DivideByZeroException);
	EXPECT_THROW((angle(Vec2d(), vec4)), DivideByZeroException);
}


