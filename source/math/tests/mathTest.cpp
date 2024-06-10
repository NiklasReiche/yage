#include <catch2/catch_all.hpp>

#include <math/maths.h>
#include <math/interpolation.h>

using namespace yage::math;
using namespace std::numbers;

TEST_CASE("RadDegConversion")
{
	auto pi_f = pi_v<float>;

	SECTION("to_rad") {
		SECTION("zero") {
			CHECK(to_rad(0.0) == 0.0);

			CHECK(to_rad(0.0f) == 0.0f);
		}
		
		SECTION("PI") {
			CHECK(to_rad(180.0) == Catch::Approx(pi));
			CHECK(to_rad(360.0) == Catch::Approx(2 * pi));
			CHECK(to_rad(540.0) == Catch::Approx(3 * pi));

			CHECK(to_rad(180.0f) == Catch::Approx(pi_f));
			CHECK(to_rad(360.0f) == Catch::Approx(2 * pi_f));
			CHECK(to_rad(540.0f) == Catch::Approx(3 * pi_f));
		}
		
		SECTION("negative") {
			CHECK(to_rad(-180.0) == Catch::Approx(-pi));

			CHECK(to_rad(-180.0f) == Catch::Approx(-pi_f));
		}
	}

	SECTION("to_deg") {
		SECTION("zero") {
			CHECK(to_deg(0.0) == 0.0);

			CHECK(to_deg(0.0f) == 0.0f);
		}

		SECTION("PI") {
			CHECK(to_deg(pi) == Catch::Approx(180.0));
			CHECK(to_deg(2 * pi) == Catch::Approx(360.0));
			CHECK(to_deg(3 * pi) == Catch::Approx(540.0));

			CHECK(to_deg(pi_f) == Catch::Approx(180.0f));
			CHECK(to_deg(2 * pi_f) == Catch::Approx(360.0f));
			CHECK(to_deg(3 * pi_f) == Catch::Approx(540.0f));
		}

		SECTION("negative") {
			CHECK(to_deg(-pi) == Catch::Approx(-180.0));

			CHECK(to_deg(-pi_f) == Catch::Approx(-180.0f));
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
