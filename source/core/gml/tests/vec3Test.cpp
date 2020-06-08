#include <catch2/catch.h>

#include <gml/vector.h>

using namespace gml;

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
