#include <catch2/catch.hpp>

#include <gml/matrix.h>

using namespace gml;

TEST_CASE("Mat2 test")
{
	SECTION("Conversion") {
		const Mat2i mat = Matrix<int, 2, 2>{
			0, 1,
			2, 3 };

		CHECK(mat == Mat2i{ 0, 1,
		                    2, 3 });
	}

	SECTION("Identity") {
		const Mat2i mat = Mat2i::Id;

		CHECK(mat == Mat2i{ 1, 0,
		                    0, 1 });
	}
}

TEST_CASE("Mat3 test")
{
	SECTION("Conversion") {
		Mat3i mat = Matrix<int, 3, 3>{
			0, 1, 2,
			3, 2, 1,
			4, 5, 6 };

		CHECK(mat == Mat3i{ 0, 1, 2,
		                    3, 2, 1,
		                    4, 5, 6 });
	}

	SECTION("Identity") {
		const Mat3i mat = Mat3i::Id;

		CHECK(mat == Mat3i{ 1, 0, 0,
		                    0, 1, 0,
		                    0, 0, 1 });
	}
}

TEST_CASE("Mat4 test")
{
	SECTION("Conversion") {
		Mat4i mat = Matrix<int, 4, 4>{
			0, 1, 2, 3,
			2, 1, 4, 5,
			6, 1, 2, 3,
			9, 8, 7, 6 };

		CHECK(mat == Mat4i{ 0, 1, 2, 3,
		                    2, 1, 4, 5,
		                    6, 1, 2, 3,
		                    9, 8, 7, 6 });
	}

	SECTION("Identity") {
		const Mat4i mat = Mat4i::Id;

		CHECK(mat == Mat4i{ 1, 0, 0, 0,
		                    0, 1, 0, 0,
		                    0, 0, 1, 0,
		                    0, 0, 0, 1});
	}

	SECTION("get scaling component") {
		const Mat4d mat = Mat4d{
			2, 0, 0, 5,
			0, 5, 0, 1,
			0, 0, 10, 8,
			0, 0, 0, 1};

		CHECK(mat.getScale() == Vec3<double>(2, 5, 10));
	}
}
