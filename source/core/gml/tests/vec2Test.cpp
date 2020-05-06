#include <gtest/gtest.h>

#include <gml/vector.h>

using namespace gml;

class Vec2Test : public testing::Test
{
};

TEST(Vec2Test, Constructor_Default)
{
	Vec2<int> vec;

	EXPECT_EQ(0, vec.x);
	EXPECT_EQ(0, vec.y);
}

TEST(Vec2Test, Constructor_InitializerList)
{
	Vec2<int> vec{25, 42};

	EXPECT_EQ(25, vec.x);
	EXPECT_EQ(42, vec.y);
}

TEST(Vec2Test, Constructor_Fill)
{
	Vec2<int> vec(3);

	EXPECT_EQ(3, vec.x);
	EXPECT_EQ(3, vec.y);
}

TEST(Vec2Test, Constructor_Explicit)
{
	Vec2<int> vec(25, 42);

	EXPECT_EQ(25, vec.x);
	EXPECT_EQ(42, vec.y);
}

TEST(Vec2Test, Constructor_Copy)
{
	Vec2<int> initial(2, 6);
	Vec2<int> vec = initial;
	initial.x = 3;
	initial.y = 9;

	EXPECT_EQ(2, vec.x);
	EXPECT_EQ(6, vec.y);
	EXPECT_EQ(3, initial.x);
	EXPECT_EQ(9, initial.y);
}

TEST(Vec2Test, Constructor_Conversion)
{
	Vec2<float> initial(2.0f, 6.0f);
	Vec2<double> vec = initial;
	initial.x = 3;
	initial.y = 9;

	EXPECT_EQ(2.0f, vec.x);
	EXPECT_EQ(6.0f, vec.y);
	EXPECT_EQ(3, initial.x);
	EXPECT_EQ(9, initial.y);
}

TEST(Vec2Test, Constructor_ConversionBase)
{
	Vec2<double> vec = Vector<float, 2>{2.0f, 6.0f};

	EXPECT_EQ(2.0f, vec.x);
	EXPECT_EQ(6.0f, vec.y);
}


TEST(Vec2Test, Assignment_Copy)
{
	Vec2<int> initial(2, 6);
	Vec2<int> vec;

	vec = initial;
	initial.x = 3;
	initial.y = 9;

	EXPECT_EQ(2, vec.x);
	EXPECT_EQ(6, vec.y);
	EXPECT_EQ(3, initial.x);
	EXPECT_EQ(9, initial.y);
}

TEST(Vec2Test, Assignment_Conversion)
{
	Vec2<double> vec;

	vec = Vec2<float>(2.0f, 6.0f);

	EXPECT_EQ(2.0f, vec.x);
	EXPECT_EQ(6.0f, vec.y);
}

TEST(Vec2Test, Assignment_ConversionBase)
{
	Vec2<double> vec;

	vec = Vector<double, 2>{ 2.0, 6.0 };

	EXPECT_EQ(2.0f, vec.x);
	EXPECT_EQ(6.0f, vec.y);
}
