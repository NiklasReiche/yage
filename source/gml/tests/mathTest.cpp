#include <catch2/catch_all.hpp>

#include <gml/maths.h>
#include <gml/interpolation.h>

using namespace gml;
using namespace std::numbers;

TEST_CASE("RadDegConversion")
{
	auto pi_f = pi_v<float>;

	SECTION("toRad") {
		SECTION("zero") {
			CHECK(toRad(0.0) == 0.0);

			CHECK(toRad(0.0f) == 0.0f);
		}
		
		SECTION("PI") {
			CHECK(toRad(180.0) == Catch::Approx(pi));
			CHECK(toRad(360.0) == Catch::Approx(2 * pi));
			CHECK(toRad(540.0) == Catch::Approx(3 * pi));

			CHECK(toRad(180.0f) == Catch::Approx(pi_f));
			CHECK(toRad(360.0f) == Catch::Approx(2 * pi_f));
			CHECK(toRad(540.0f) == Catch::Approx(3 * pi_f));
		}
		
		SECTION("negative") {
			CHECK(toRad(-180.0) == Catch::Approx(-pi));

			CHECK(toRad(-180.0f) == Catch::Approx(-pi_f));
		}
	}

	SECTION("toDeg") {
		SECTION("zero") {
			CHECK(toDeg(0.0) == 0.0);

			CHECK(toDeg(0.0f) == 0.0f);
		}

		SECTION("PI") {
			CHECK(toDeg(pi) == Catch::Approx(180.0));
			CHECK(toDeg(2 * pi) == Catch::Approx(360.0));
			CHECK(toDeg(3 * pi) == Catch::Approx(540.0));

			CHECK(toDeg(pi_f) == Catch::Approx(180.0f));
			CHECK(toDeg(2 * pi_f) == Catch::Approx(360.0f));
			CHECK(toDeg(3 * pi_f) == Catch::Approx(540.0f));
		}

		SECTION("negative") {
			CHECK(toDeg(-pi) == Catch::Approx(-180.0));

			CHECK(toDeg(-pi_f) == Catch::Approx(-180.0f));
		}
	}
}

TEST_CASE("Lerp")
{
	SECTION("default") {
		CHECK(lerp(0.0, 1.0, 0.5) == Catch::Approx(0.5));
		CHECK(lerp(0.0, 3.0, 1 / 3.0) == Catch::Approx(1));
		CHECK(lerp(0, 10, 0.5) == 5);
	}

	SECTION("negative values") {
		CHECK(lerp(-3.0, 1.0, 0.5) == Catch::Approx(-1));
	}

	SECTION("zero range") {
		CHECK(lerp(0.0, 0.0, 0.5) == Catch::Approx(0));
	}

	SECTION("overflow") {
		CHECK(lerp(0.0, 1.0, 2.0) == 1.0);
	}

	SECTION("underflow") {
		CHECK(lerp(0.0, 1.0, -1.0) == 0.0);
	};
}
