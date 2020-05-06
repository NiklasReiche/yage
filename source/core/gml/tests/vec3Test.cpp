#include <gtest/gtest.h>

#include <gml/vector.h>

using namespace gml;

class Vec3Test : public testing::Test
{
};

TEST(Vec3Test, Constructor_Default)
{
	Vec3<int> vec;

	EXPECT_EQ(0, vec.x);
	EXPECT_EQ(0, vec.y);
	EXPECT_EQ(0, vec.z);
}

TEST(Vec3Test, Constructor_InitializerList)
{
	Vec3<int> vec{ 25, 42, 5 };

	EXPECT_EQ(25, vec.x);
	EXPECT_EQ(42, vec.y);
	EXPECT_EQ(5, vec.z);
}

TEST(Vec3Test, Constructor_Fill)
{
	Vec3<int> vec(3);

	EXPECT_EQ(3, vec.x);
	EXPECT_EQ(3, vec.y);
	EXPECT_EQ(3, vec.z);
}

TEST(Vec3Test, Constructor_Explicit)
{
	Vec3<int> vec(25, 42, 5);

	EXPECT_EQ(25, vec.x);
	EXPECT_EQ(42, vec.y);
	EXPECT_EQ(5, vec.z);
}

TEST(Vec3Test, Constructor_Copy)
{
	Vec3<int> initial(2, 6, 5);
	Vec3<int> vec = initial;
	initial.x = 3;
	initial.y = 9;
	initial.z = 1;

	EXPECT_EQ(2, vec.x);
	EXPECT_EQ(6, vec.y);
	EXPECT_EQ(5, vec.z);
	EXPECT_EQ(3, initial.x);
	EXPECT_EQ(9, initial.y);
	EXPECT_EQ(1, initial.z);
}

TEST(Vec3Test, Constructor_Conversion)
{
	Vec3<float> initial(2.0f, 6.0f, 5.0f);
	Vec3<double> vec = initial;
	initial.x = 3;
	initial.y = 9;
	initial.z = 1;

	EXPECT_EQ(2.0, vec.x);
	EXPECT_EQ(6.0, vec.y);
	EXPECT_EQ(5.0, vec.z);
	EXPECT_EQ(3, initial.x);
	EXPECT_EQ(9, initial.y);
	EXPECT_EQ(1, initial.z);
}

TEST(Vec3Test, Constructor_ConversionBase)
{
	Vec3<double> vec = Vector<float, 3>{ 2.0f, 6.0f, 5.0f };

	EXPECT_EQ(2.0f, vec.x);
	EXPECT_EQ(6.0f, vec.y);
	EXPECT_EQ(5.0f, vec.z);
}


TEST(Vec3Test, Assignment_Copy)
{
	Vec3<double> initial = Vec3<double>(2.0, 6.0, 1.0);
	Vec3<double> vec;

	vec = initial;

	initial.x = 3;
	initial.y = 9;
	initial.z = 1;

	EXPECT_EQ(2.0, vec.x);
	EXPECT_EQ(6.0, vec.y);
	EXPECT_EQ(1.0, vec.z);
	EXPECT_EQ(3, initial.x);
	EXPECT_EQ(9, initial.y);
	EXPECT_EQ(1, initial.z);
}

TEST(Vec3Test, Assignment_Conversion)
{
	Vec3<double> vec;

	vec = Vec3<float>(2.0f, 6.0f, 1.0f);

	EXPECT_EQ(2.0f, vec.x);
	EXPECT_EQ(6.0f, vec.y);
	EXPECT_EQ(1.0f, vec.z);
}

TEST(Vec3Test, Assignment_ConversionBase)
{
	Vec3<double> vec;

	vec = Vector<double, 3>{ 2.0, 6.0, 1.0 };

	EXPECT_EQ(2.0, vec.x);
	EXPECT_EQ(6.0, vec.y);
	EXPECT_EQ(1.0, vec.z);
}
