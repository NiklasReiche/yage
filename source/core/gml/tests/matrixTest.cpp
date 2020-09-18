#include <catch2/catch.hpp>

#include <gml/matrix.h>

using namespace gml;

TEST_CASE("MatrixBase test")
{
	SECTION("Constructor") {
		SECTION("Constructor_Default") {
			const Matrix<int, 2, 3> mat;

			CHECK(0 == mat(0, 0));
			CHECK(0 == mat(0, 1));
			CHECK(0 == mat(0, 2));

			CHECK(0 == mat(1, 0));
			CHECK(0 == mat(1, 1));
			CHECK(0 == mat(1, 2));
		}

		SECTION("Constructor_Fill") {
			const Matrix<int, 3, 2> mat(42);

			CHECK(42 == mat(0, 0));
			CHECK(42 == mat(0, 1));

			CHECK(42 == mat(1, 0));
			CHECK(42 == mat(1, 1));

			CHECK(42 == mat(2, 0));
			CHECK(42 == mat(2, 1));
		}

		SECTION("Constructor_Array") {
			int arr[] = {
				3, 5, 2,
				6, 3, 8
			};

			const Matrix<int, 2, 3> mat(arr);

			CHECK(3 == mat(0, 0));
			CHECK(5 == mat(0, 1));
			CHECK(2 == mat(0, 2));

			CHECK(6 == mat(1, 0));
			CHECK(3 == mat(1, 1));
			CHECK(8 == mat(1, 2));
		}


		SECTION("Constructor_InitializerList1D") {
			const Matrix<int, 2, 3> mat{
				3, 5, 2,
				6, 3, 8
			};

			CHECK(3 == mat(0, 0));
			CHECK(5 == mat(0, 1));
			CHECK(2 == mat(0, 2));

			CHECK(6 == mat(1, 0));
			CHECK(3 == mat(1, 1));
			CHECK(8 == mat(1, 2));
		}

		SECTION("Constructor_InitializerList2D") {
			const Matrix<int, 2, 3> mat{
				{ 3, 5, 2 },
				{ 6, 3, 8 }
			};

			CHECK(3 == mat(0, 0));
			CHECK(5 == mat(0, 1));
			CHECK(2 == mat(0, 2));

			CHECK(6 == mat(1, 0));
			CHECK(3 == mat(1, 1));
			CHECK(8 == mat(1, 2));
		}

		SECTION("Constructor_Copy") {
			const Matrix<int, 2, 3> initial{
				3, 5, 2,
				6, 3, 8
			};

			const Matrix<int, 2, 3> mat = initial;

			CHECK(3 == mat(0, 0));
			CHECK(5 == mat(0, 1));
			CHECK(2 == mat(0, 2));

			CHECK(6 == mat(1, 0));
			CHECK(3 == mat(1, 1));
			CHECK(8 == mat(1, 2));
		}

		SECTION("Constructor_Conversion") {
			const Matrix<float, 2, 3> initial{
				3.0f, 5.0f, 2.0f,
				6.0f, 3.0f, 8.0f
			};

			const Matrix<double, 2, 3> mat = initial;

			CHECK(3.0 == mat(0, 0));
			CHECK(5.0 == mat(0, 1));
			CHECK(2.0 == mat(0, 2));

			CHECK(6.0 == mat(1, 0));
			CHECK(3.0 == mat(1, 1));
			CHECK(8.0 == mat(1, 2));
		}
	}

	SECTION("Operators") {
		SECTION("Operator_Add") {
			const Matrix<int, 2, 3> lhs = {
				{ 1, 3, 0 },
				{ 3, 6, 1 }};

			const Matrix<int, 2, 3> rhs = {
				{ -1, 2, 0 },
				{ 3,  7, 4 }};

			const Matrix<int, 2, 3> expected = {
				{ 0, 5,  0 },
				{ 6, 13, 5 }};

			CHECK(expected == lhs + rhs);
		}

		SECTION("Operator_Subtract") {
			const Matrix<int, 2, 3> lhs = {
				{ 1, 3, 0 },
				{ 3, 6, 1 }};

			const Matrix<int, 2, 3> rhs = {
				{ -1, 2, 0 },
				{ 3,  7, 4 }};

			const Matrix<int, 2, 3> expected = {
				{ 2, 1,  0 },
				{ 0, -1, -3 }};

			CHECK(expected == lhs - rhs);
		}

		SECTION("Operator_Multiply") {
			const Matrix<int, 2, 3> lhs = {
				{ 1, 3, 0 },
				{ 3, 6, 1 }};

			const Matrix<int, 3, 4> rhs = {
				{ -1, 2, 0, 1 },
				{ -1, 2, 0, 4 },
				{ 3,  7, 4, 2 }};

			const Matrix<int, 2, 4> expected = {
				{ -4, 8,  0, 13 },
				{ -6, 25, 4, 29 }};

			CHECK(expected == lhs * rhs);
		}

		SECTION("Operator_Divide") {
			const Matrix<int, 2, 3> lhs = {
				{ 2, 6,  0 },
				{ 6, 12, 2 }};

			const Matrix<int, 2, 3> expected = {
				{ 1, 3, 0 },
				{ 3, 6, 1 }};

			CHECK(expected == lhs / 2);
		}
	}

	SECTION("Identity") {
		Matrix<int, 2, 2> id = gml::Id<int, 2>;

		CHECK(id(0, 0) == 1);
		CHECK(id(0, 1) == 0);
		CHECK(id(1, 0) == 0);
		CHECK(id(1, 1) == 1);
	}

	SECTION("determinant") {
		SECTION("Det_1x1") {
			const Matrix<int, 1, 1> mat(7);

			CHECK(7 == det(mat));
		}

		SECTION("Det_2x2") {
			const Matrix<double, 2, 2> mat{
				1, 2,
				3, 4
			};

			CHECK(-2 == det(mat));
		}

		SECTION("Det_3x3") {
			const Matrix<double, 3, 3> mat{
				2, -1, 0,
				-1, 2, -1,
				0, -1, 2
			};

			CHECK(4 == det(mat));
		}

		SECTION("Det_HigherDimension") {
			const Matrix<float, 4, 4> mat = {
				{ 0,  1,  3,   2 },
				{ 2,  -1, -9,  -2 },
				{ 1,  7,  2,   9 },
				{ -1, 6,  -11, 4 }
			};

			CHECK(64 == det(mat));
		}
	}

	SECTION("Trace") {
		const Matrix<int, 3, 3> mat{
			2, 4, 1,
			6, -1, 0,
			7, 3, 5
		};

		CHECK(6 == trace(mat));
	}

	SECTION("transpose") {
		SECTION("Transpose_NonQuadratic") {
			const Matrix<int, 2, 3> mat = {
				{ 2, 3,  0 },
				{ 6, 18, 2 }
			};

			const Matrix<int, 3, 2> expected = {
				{ 2, 6 },
				{ 3, 18 },
				{ 0, 2 }
			};

			CHECK(expected == transpose(mat));
			CHECK(mat == transpose(expected));
			CHECK(mat == transpose(transpose(mat)));
		}

		SECTION("Transpose_Quadratic") {
			const Matrix<int, 2, 2> mat{
				1, 2,
				3, 4
			};

			const Matrix<int, 2, 2> trans{
				1, 3,
				2, 4
			};

			CHECK(trans == transpose(mat));
			CHECK(mat == transpose(trans));
			CHECK(mat == transpose(transpose(mat)));
		}
	}
	
	SECTION("Inverse") {
		SECTION("Inverse_2x2") {
			const Matrix<double, 2, 2> mat{
				1, 2,
				3, 4
			};

			const Matrix<double, 2, 2> inv{
				-2, 1,
				1.5, -0.5
			};

			CHECK(inv == inverse(mat));
			CHECK(mat == inverse(inv));
			CHECK(mat == inverse(inverse(mat)));
		}

		SECTION("Inverse_3x3") {
			const Matrix<double, 3, 3> mat{
				2, -1, 0,
				-1, 2, -1,
				0, -1, 2
			};

			const Matrix<double, 3, 3> inv{
				3 / 4.0, 2 / 4.0, 1 / 4.0,
				2 / 4.0, 4 / 4.0, 2 / 4.0,
				1 / 4.0, 2 / 4.0, 3 / 4.0
			};

			CHECK(inv == inverse(mat));
			CHECK(mat == inverse(inv));
			CHECK(mat == inverse(inverse(mat)));
		}

		SECTION("Inverse_higherDimension") {
			const Matrix<double, 4, 4> mat{
				{ 4,  1,  3,   2 },
				{ 2,  -1, -9,  -2 },
				{ 1,  7,  2,   9 },
				{ -1, 6,  -11, 4 }
			};

			const Matrix<double, 4, 4> inv{
				{ 163 / 588.0, 1 / 84.0,   -4 / 49.0,  5 / 98.0 },
				{ 349 / 588.0, -53 / 84.0, -26 / 49.0, 57 / 98.0 },
				{ 65 / 588.0,  -13 / 84.0, -4 / 49.0,  5 / 98.0 },
				{ -76 / 147.0, 11 / 21.0,  27 / 49.0,  -23 / 49.0 }
			};

			auto expect_eq = [](const Matrix<double, 4, 4>& a, const Matrix<double, 4, 4>& b)
			{
				for (int i = 0; i < 4; ++i) {
					for (int j = 0; j < 4; ++j) {
						CHECK(a(i, j) == Approx(b(i, j)).epsilon(1e-10));
					}
				}
			};

			expect_eq(inv, inverse(mat));
			expect_eq(mat, inverse(inv));
			expect_eq(mat, inverse(inverse(mat)));
		}
	}
}
