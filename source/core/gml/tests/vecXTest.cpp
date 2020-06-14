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

TEST_CASE("Vec3 test")
{
	SECTION("Constructor") {
		SECTION("Default") {
			Vec3<int> vec;

			CHECK(0 == vec.x);
			CHECK(0 == vec.y);
			CHECK(0 == vec.z);
		}

		SECTION("InitializerList") {
			Vec3<int> vec{ 25, 42, 5 };

			CHECK(25 == vec.x);
			CHECK(42 == vec.y);
			CHECK(5 == vec.z);
		}

		SECTION("Fill") {
			Vec3<int> vec(3);

			CHECK(3 == vec.x);
			CHECK(3 == vec.y);
			CHECK(3 == vec.z);
		}

		SECTION("Explicit") {
			Vec3<int> vec(25, 42, 5);

			CHECK(25 == vec.x);
			CHECK(42 == vec.y);
			CHECK(5 == vec.z);
		}

		SECTION("Copy") {
			Vec3<int> initial(2, 6, 5);
			Vec3<int> vec = initial;
			initial.x = 3;
			initial.y = 9;
			initial.z = 1;

			CHECK(2 == vec.x);
			CHECK(6 == vec.y);
			CHECK(5 == vec.z);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
			CHECK(1 == initial.z);
		}

		SECTION("Conversion") {
			Vec3<float> initial(2.0f, 6.0f, 5.0f);
			Vec3<double> vec = initial;
			initial.x = 3;
			initial.y = 9;
			initial.z = 1;

			CHECK(2.0 == vec.x);
			CHECK(6.0 == vec.y);
			CHECK(5.0 == vec.z);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
			CHECK(1 == initial.z);
		}

		SECTION("ConversionBase") {
			Vec3<double> vec = Vector<float, 3>{ 2.0f, 6.0f, 5.0f };

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
			CHECK(5.0f == vec.z);
		}
	}

	SECTION("Assignment") {
		SECTION("Copy") {
			Vec3<double> initial = Vec3<double>(2.0, 6.0, 1.0);
			Vec3<double> vec;

			vec = initial;

			initial.x = 3;
			initial.y = 9;
			initial.z = 1;

			CHECK(2.0 == vec.x);
			CHECK(6.0 == vec.y);
			CHECK(1.0 == vec.z);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
			CHECK(1 == initial.z);
		}

		SECTION("Conversion") {
			Vec3<double> vec;

			vec = Vec3<float>(2.0f, 6.0f, 1.0f);

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
			CHECK(1.0f == vec.z);
		}

		SECTION("ConversionBase") {
			Vec3<double> vec;

			vec = Vector<double, 3>{ 2.0, 6.0, 1.0 };

			CHECK(2.0 == vec.x);
			CHECK(6.0 == vec.y);
			CHECK(1.0 == vec.z);
		}
	}
}

TEST_CASE("Vec4 test")
{
	SECTION("Constructor") {
		SECTION("Default") {
			Vec4<int> vec;

			CHECK(0 == vec.x);
			CHECK(0 == vec.y);
			CHECK(0 == vec.z);
			CHECK(0 == vec.w);
		}

		SECTION("InitializerList") {
			Vec4<int> vec{ 25, 42, 5, 1 };

			CHECK(25 == vec.x);
			CHECK(42 == vec.y);
			CHECK(5 == vec.z);
			CHECK(1 == vec.w);
		}

		SECTION("Fill") {
			Vec4<int> vec(42);

			CHECK(42 == vec.x);
			CHECK(42 == vec.y);
			CHECK(42 == vec.z);
			CHECK(42 == vec.w);
		}

		SECTION("Explicit") {
			Vec4<int> vec(25, 42, 5, 3);

			CHECK(25 == vec.x);
			CHECK(42 == vec.y);
			CHECK(5 == vec.z);
			CHECK(3 == vec.w);
		}

		SECTION("Copy") {
			Vec4<int> initial(2, 6, 5, 10);
			Vec4<int> vec = initial;
			initial.x = 3;
			initial.y = 9;
			initial.z = 1;
			initial.w = 8;

			CHECK(2 == vec.x);
			CHECK(6 == vec.y);
			CHECK(5 == vec.z);
			CHECK(10 == vec.w);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
			CHECK(1 == initial.z);
			CHECK(8 == initial.w);
		}

		SECTION("Conversion") {
			Vec4<float> initial(2.0f, 6.0f, 5.0f, 42.0f);
			Vec4<double> vec = initial;
			initial.x = 3;
			initial.y = 9;
			initial.z = 1;
			initial.w = -1;

			CHECK(2.0 == vec.x);
			CHECK(6.0 == vec.y);
			CHECK(5.0 == vec.z);
			CHECK(42.0 == vec.w);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
			CHECK(1 == initial.z);
			CHECK(-1 == initial.w);
		}

		SECTION("ConversionBase") {
			Vec4<double> vec = Vector<float, 4>{ 2.0f, 6.0f, 5.0f, 1.0f };

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
			CHECK(5.0f == vec.z);
			CHECK(1.0f == vec.w);
		}
	}

	SECTION("Assignment") {
		SECTION("Copy") {
			Vec4<double> initial = Vec4<double>(2.0, 6.0, 1.0, 42.0f);
			Vec4<double> vec;

			vec = initial;

			initial.x = 3;
			initial.y = 9;
			initial.z = 1;
			initial.w = -2;

			CHECK(2.0 == vec.x);
			CHECK(6.0 == vec.y);
			CHECK(1.0 == vec.z);
			CHECK(42.0 == vec.w);
			CHECK(3 == initial.x);
			CHECK(9 == initial.y);
			CHECK(1 == initial.z);
			CHECK(-2 == initial.w);
		}

		SECTION("Conversion") {
			Vec4<double> vec;

			vec = Vec4<float>(2.0f, 6.0f, 1.0f, 5.0f);

			CHECK(2.0f == vec.x);
			CHECK(6.0f == vec.y);
			CHECK(1.0f == vec.z);
			CHECK(5.0f == vec.w);
		}

		SECTION("ConversionBase") {
			Vec4<double> vec;

			vec = Vector<double, 4>{ 2.0, 6.0, 1.0, 5.0f };

			CHECK(2.0 == vec.x);
			CHECK(6.0 == vec.y);
			CHECK(1.0 == vec.z);
			CHECK(5.0 == vec.w);
		}
	}
}
