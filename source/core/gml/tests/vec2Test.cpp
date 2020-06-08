#include <catch2/catch.h>

#include <gml/vector.h>

using namespace gml;

TEST_CASE("Vec2 test")
{
	SECTION("Constructor") {
		SECTION("Constructor_Default") {
			Vec2<int> vec;

			CHECK(0 == vec.x);
			CHECK(0 == vec.y);
		}

		SECTION("Constructor_InitializerList") {
			Vec2<int> vec{ 25, 42 };

			CHECK(25 == vec.x);
			CHECK(42 == vec.y);
		}

		SECTION("Constructor_Fill") {
			Vec2<int> vec(3);

			CHECK(3 == vec.x);
			CHECK(3 == vec.y);
		}

		SECTION("Constructor_Explicit") {
			Vec2<int> vec(25, 42);

			CHECK(25 == vec.x);
			CHECK(42 == vec.y);
		}

		SECTION("Constructor_Copy") {
			Vec2<int> initial(2, 6);
			Vec2<int> vec = initial;
			initial.x = 3;
			initial.y = 9;

			CHECK(2 == vec.x);
			CHECK(6 == vec.y);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
		}

		SECTION("Constructor_Conversion") {
			Vec2<float> initial(2.0f, 6.0f);
			Vec2<double> vec = initial;
			initial.x = 3;
			initial.y = 9;

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
		}

		SECTION("Constructor_ConversionBase") {
			Vec2<double> vec = Vector<float, 2>{ 2.0f, 6.0f };

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
		}
	}
	
	SECTION("Assignment") {
		SECTION("Assignment_Copy") {
			Vec2<int> initial(2, 6);
			Vec2<int> vec;

			vec = initial;
			initial.x = 3;
			initial.y = 9;

			CHECK(2 == vec.x);
			CHECK(6 == vec.y);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
		}

		SECTION("Assignment_Conversion") {
			Vec2<double> vec;

			vec = Vec2<float>(2.0f, 6.0f);

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
		}

		SECTION("Assignment_ConversionBase") {
			Vec2<double> vec;

			vec = Vector<double, 2>{ 2.0, 6.0 };

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
		}
	}
}
