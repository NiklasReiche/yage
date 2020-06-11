#include <catch2/catch.h>

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
			CHECK(toRad(180.0) == Approx(pi));
			CHECK(toRad(360.0) == Approx(2 * pi));
			CHECK(toRad(540.0) == Approx(3 * pi));

			CHECK(toRad(180.0f) == Approx(pi_f));
			CHECK(toRad(360.0f) == Approx(2 * pi_f));
			CHECK(toRad(540.0f) == Approx(3 * pi_f));
		}
		
		SECTION("negative") {
			CHECK(toRad(-180.0) == Approx(-pi));

			CHECK(toRad(-180.0f) == Approx(-pi_f));
		}
	}

	SECTION("toDeg") {
		SECTION("zero") {
			CHECK(toDeg(0.0) == 0.0);

			CHECK(toDeg(0.0f) == 0.0f);
		}

		SECTION("PI") {
			CHECK(toDeg(pi) == Approx(180.0));
			CHECK(toDeg(2 * pi) == Approx(360.0));
			CHECK(toDeg(3 * pi) == Approx(540.0));

			CHECK(toDeg(pi_f) == Approx(180.0f));
			CHECK(toDeg(2 * pi_f) == Approx(360.0f));
			CHECK(toDeg(3 * pi_f) == Approx(540.0f));
		}

		SECTION("negative") {
			CHECK(toDeg(-pi) == Approx(-180.0));

			CHECK(toDeg(-pi_f) == Approx(-180.0f));
		}
	}
}

TEST_CASE("Lerp")
{
	CHECK(gml::lerp(0.0, 1.0, 0.5) == Approx(0.5));
	CHECK(gml::lerp(0.0, 0.0, 0.5) == Approx(0));
	CHECK(gml::lerp(0.0, 3.0, 1/3.0) == Approx(1));
	CHECK(gml::lerp(-3.0, 1.0, 0.5) == Approx(-1));

	CHECK(gml::lerp(0, 10, 0.5) == 5);
}
