#include <gtest/gtest.h>

#include <math/matrix.h>

using namespace gml;

class MatrixTest : public testing::Test
{
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

TEST(MatrixTest, Construct)
{
	// default constructor
	Matrix<int, 2, 3> mat1;
	EXPECT_EQ(1, mat1.at(0, 0));
	EXPECT_EQ(0, mat1.at(0, 1));
	EXPECT_EQ(0, mat1.at(0, 2));
	EXPECT_EQ(0, mat1.at(1, 0));
	EXPECT_EQ(1, mat1.at(1, 1));
	EXPECT_EQ(0, mat1.at(1, 2));

	// single value constructor
	Matrix<int, 2, 2> mat2(3);
	EXPECT_EQ(3, mat2.at(0, 0));
	EXPECT_EQ(3, mat2.at(0, 1));
	EXPECT_EQ(3, mat2.at(1, 0));
	EXPECT_EQ(3, mat2.at(1, 1));

	// 1d initializer list
	Matrix<int, 2, 3> mat3 = {
		1, 3, 0,
		3, 6, 1};
	EXPECT_EQ(1, mat3.at(0, 0));
	EXPECT_EQ(3, mat3.at(0, 1));
	EXPECT_EQ(0, mat3.at(0, 2));
	EXPECT_EQ(3, mat3.at(1, 0));
	EXPECT_EQ(6, mat3.at(1, 1));
	EXPECT_EQ(1, mat3.at(1, 2));

	// 2d initializer list
	Matrix<int, 2, 3> mat4 = {
		{ 1, 3, 0 },
		{ 3, 6, 1 }};
	EXPECT_EQ(1, mat4.at(0, 0));
	EXPECT_EQ(3, mat4.at(0, 1));
	EXPECT_EQ(0, mat4.at(0, 2));
	EXPECT_EQ(3, mat4.at(1, 0));
	EXPECT_EQ(6, mat4.at(1, 1));
	EXPECT_EQ(1, mat4.at(1, 2));

	// conversion constructor
	Mat2i mat5 = Matrix<int, 2, 2>(3);
	EXPECT_EQ(mat2, mat5);

	mat5 = mat2;
	EXPECT_EQ(mat2, mat5);
}

TEST(MatrixTest, Add)
{
	Matrix<int, 2, 3> mat1 = {
		{ 1, 3, 0 },
		{ 3, 6, 1 } };

	Matrix<int, 2, 3> mat2 = {
		{ -1, 2, 0 },
		{ 3, 7, 4 } };

	Matrix<int, 2, 3> result = mat1 + mat2;
	Matrix<int, 2, 3> expected = {
		{ 0, 5, 0 },
		{ 6, 13, 5 } };

	EXPECT_EQ(expected, result);
}

TEST(MatrixTest, Subtract)
{
	Matrix<int, 2, 3> mat1 = {
		{ 1, 3, 0 },
		{ 3, 6, 1 } };

	Matrix<int, 2, 3> mat2 = {
		{ -1, 2, 0 },
		{ 3, 7, 4 } };

	Matrix<int, 2, 3> result = mat1 - mat2;
	Matrix<int, 2, 3> expected = {
		{ 2, 1, 0 },
		{ 0, -1, -3 } };

	EXPECT_EQ(expected, result);
}

TEST(MatrixTest, Multiply)
{
	Matrix<int, 2, 3> mat1 = {
		{ 1, 3, 0 },
		{ 3, 6, 1 } };

	Matrix<int, 3, 3> mat2 = {
		{ -1, 2, 0 },
		{ -1, 2, 0 },
		{  3, 7, 4 } };

	Matrix<int, 2, 3> result = mat1 * mat2;
	Matrix<int, 2, 3> expected = {
		{ -4,  8, 0 },
		{ -6, 25, 4 } };

	EXPECT_EQ(expected, result);

	result = mat1 * 2;
	expected = {
		{ 2,  6, 0 },
		{ 6, 12, 2 } };

	EXPECT_EQ(expected, result);
}

TEST(MatrixTest, Divide)
{
	Matrix<int, 2, 3> mat1 = {
		{ 2,  6, 0 },
		{ 6, 12, 2 } };

	Matrix<int, 2, 3> expected = {
		{ 1, 3, 0 },
		{ 3, 6, 1 } };

	Matrix<int, 2, 3> result = mat1 / 2;
	EXPECT_EQ(expected, result);
}

TEST(MatrixTest, Det)
{
	Mat4f mat1 = {
		{ 0,  1,  3,  2 },
		{ 2, -1, -9, -2 },
		{ 1,  7,  2,  9 },
		{ -1, 6, -11, 4 }
	};

	EXPECT_EQ(64, mat1.det());
}

TEST(MatrixTest, Inverse)
{
	Mat4f mat1{
		{  4,  1,  3,  2 },
		{  2, -1, -9, -2 },
		{  1,  7,  2,  9 },
		{ -1,  6, -11, 4 } 
	};

	Mat4f expected{
		{ 163 / 588.0f,   1 / 84.0f,  -4 / 49.0f,   5 / 98.0f },
		{ 349 / 588.0f, -53 / 84.0f, -26 / 49.0f,  57 / 98.0f },
		{  65 / 588.0f, -13 / 84.0f,  -4 / 49.0f,   5 / 98.0f },
		{ -76 / 147.0f,  11 / 21.0f,  27 / 49.0f, -23 / 49.0f } 
	};

	Mat4f result = inverse(mat1);

	for (auto i = 0; i < 4; ++i)
	{
		for (auto j = 0; j < 4; ++j)
		{
			EXPECT_NEAR(expected.at(i, j), result.at(i, j), 0.00001);
		}
	}

	mat1 = {
		{ 0,  1,  3,  2 },
		{ 2, -1, -9, -2 },
		{ 1,  7,  2,  9 },
		{ -1, 6, -11, 4 } 
	};

	expected = (1 / 64.0f) * Mat4f{
		{ -163,  -7,  48, -30 }, 
		{ -349, -57,  80, -34 }, 
		{  -65, -13,  16, -10 }, 
		{  304,  48, -64,  32 }
	};

	result = inverse(mat1);

	for (auto i = 0; i < 4; ++i)
	{
		for (auto j = 0; j < 4; ++j)
		{
			EXPECT_NEAR(expected.at(i, j), result.at(i, j), 0.00001);
		}
	}
}

TEST(MatrixTest, Transpose)
{
	Matrix<int, 2, 3> mat1 = {
		{ 2,  3, 0 },
		{ 6, 18, 2 } };

	Matrix<int, 3, 2> expected = {
		{ 2,  6},
		{ 3, 18 },
		{ 0,  2 } };

	EXPECT_EQ(expected, transpose(mat1));
}
