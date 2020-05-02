#include <gtest/gtest.h>

#include <gml/maths.h>
#include <gml/vector.h>

using namespace gml;

class VectorTest : public testing::Test
{};

TEST(VectorTest, Construct)
{
	// default constructor
	Vector<int, 5> vec1;
	EXPECT_EQ(0, vec1.at(0));
	EXPECT_EQ(0, vec1.at(1));
	EXPECT_EQ(0, vec1.at(2));
	EXPECT_EQ(0, vec1.at(3));
	EXPECT_EQ(0, vec1.at(4));

	// initializer list constructor
	Vector<float, 3> vec2{ 2.0f, -1.5f, 0.0f };
	EXPECT_EQ(2.0f, vec2.at(0));
	EXPECT_EQ(-1.5f, vec2.at(1));
	EXPECT_EQ(0.0f, vec2.at(2));

	// initializer list constructor
	Vector<double, 4> vec3 = { 0.0, 1.25, 10.0, -1.3 };
	EXPECT_EQ(0.0, vec3.at(0));
	EXPECT_EQ(1.25, vec3.at(1));
	EXPECT_EQ(10.0, vec3.at(2));
	EXPECT_EQ(-1.3, vec3.at(3));

	EXPECT_THROW((Vector<int, 1>{ 0, 1 }), InvalidDimensionException);

	// single value constructor
	Vector<int, 2> vec4(4);
	EXPECT_EQ(4, vec4.at(0));
	EXPECT_EQ(4, vec4.at(1));

	// assignment opeator
	Vec3i vec5 = { 1,2,3 };
	Vec3i vec6(1);
	vec6 = vec5;
	EXPECT_EQ(1, vec6.x);
	EXPECT_EQ(2, vec6.y);
	EXPECT_EQ(3, vec6.z);

	// conversion constructor
	Vector<int, 2> vec7 = Vector<int, 2>(1);
	EXPECT_EQ(1, vec7.at(0));
	EXPECT_EQ(1, vec7.at(1));
	Vec2i vec8 = Vector<int, 2>(1);
	EXPECT_EQ(1, vec8.x);
	EXPECT_EQ(1, vec8.y);
	Vector<int, 2> vec9 = Vec2i(1);
	EXPECT_EQ(1, vec9.at(0));
	EXPECT_EQ(1, vec9.at(1));
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

	double expected = -1;
	double result = 0;

	result = dot(vec1, vec2);
	EXPECT_EQ(expected, result);
}

TEST(VectorTest, Normalize)
{
	Vec3d vec1(2, 1, 3);

	Vec3d expected(2 / std::sqrt(14), 1 / std::sqrt(14), 3 / std::sqrt(14));

	EXPECT_EQ(std::sqrt(14), vec1.length());

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

	double expected = toRad(45);
	double result = angle(vec1, vec2);
	EXPECT_DOUBLE_EQ(expected, result);
	EXPECT_THROW((angle(Vec3d(), Vec3d())), DivideByZeroException);
	EXPECT_THROW((angle(vec1, Vec3d())), DivideByZeroException);
	EXPECT_THROW((angle(Vec3d(), vec2)), DivideByZeroException);

	Vec2d vec3(1, 0);
	Vec2d vec4(1, 1);

	expected = toRad(45);
	result = angle(vec3, vec4);
	EXPECT_DOUBLE_EQ(expected, result);
	EXPECT_THROW((angle(Vec2d(), Vec2d())), DivideByZeroException);
	EXPECT_THROW((angle(vec3, Vec2d())), DivideByZeroException);
	EXPECT_THROW((angle(Vec2d(), vec4)), DivideByZeroException);
}
