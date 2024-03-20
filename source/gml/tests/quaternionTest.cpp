#include <catch2/catch_all.hpp>

#include <gml/quaternion.h>
#include <gml/matrix.h>

using namespace gml;
using namespace std::numbers;

template<typename T>
void EXPECT_QUAT(Quaternion<T> expected, Quaternion<T> result)
{
	CHECK(expected.w == Catch::Approx(result.w));
	CHECK(expected.x == Catch::Approx(result.x));
	CHECK(expected.y == Catch::Approx(result.y));
	CHECK(expected.z == Catch::Approx(result.z));
}
template<typename T, size_t size>
static void EXPECT_VEC(Vector<T, size> expected, Vector<T, size> result)
{
	for (size_t i = 0; i < size; ++i)
	{
		CHECK(Catch::Approx(expected(i)).margin(1e-12) == result(i));
	}
}

TEST_CASE("quaternion test")
{
	SECTION("euler") {
		Quatd q;
		SECTION("zero") {
			q = quaternion::eulerAngle<double>(0, 0, 0);
			EXPECT_QUAT(Quatd(1, 0, 0, 0), q);
		}

		SECTION("yaw") {
			q = quaternion::eulerAngle<double>(0.5 * pi, 0, 0);
			EXPECT_QUAT(Quatd(0.7071, 0, 0.7071, 0), q);
			CHECK(Catch::Approx(0.5 * pi) == q.getYaw());
			CHECK(Catch::Approx(0) == q.getRoll());
			CHECK(Catch::Approx(0) == q.getPitch());
		}

		SECTION("roll") {
			q = quaternion::eulerAngle<double>(0, 0.5 * pi, 0);
			EXPECT_QUAT(Quatd(0.7071, 0, 0, 0.7071), q);
			CHECK(Catch::Approx(0) == q.getYaw());
			CHECK(Catch::Approx(0.5 * pi) == q.getRoll());
			CHECK(Catch::Approx(0) == q.getPitch());
		}

		SECTION("pitch") {
			q = quaternion::eulerAngle<double>(0, 0, 0.5 * pi);
			EXPECT_QUAT(Quatd(0.7071, 0.7071, 0, 0), q);
			CHECK(Catch::Approx(0) == q.getYaw());
			CHECK(Catch::Approx(0) == q.getRoll());
			CHECK(Catch::Approx(0.5 * pi) == q.getPitch());
		}
	}

	SECTION("direction") {
		Quatd q;
		SECTION("zero") {
			q = quaternion::eulerAngle<double>(0, 0, 0);
			EXPECT_VEC(Vec3d(0, 0, 1), q.getForward());
			EXPECT_VEC(Vec3d(0, 1, 0), q.getUp());
			EXPECT_VEC(Vec3d(-1, 0, 0), q.getRight());
		}

		SECTION("yaw") {
			q = quaternion::eulerAngle<double>(0.5 * pi, 0, 0);
			EXPECT_VEC(Vec3d(1, 0, 0), q.getForward());
			EXPECT_VEC(Vec3d(0, 1, 0), q.getUp());
			EXPECT_VEC(Vec3d(0, 0, 1), q.getRight());
		}

		SECTION("roll") {
			q = quaternion::eulerAngle<double>(0, 0.5 * pi, 0);
			EXPECT_VEC(Vec3d(0, 0, 1), q.getForward());
			EXPECT_VEC(Vec3d(-1, 0, 0), q.getUp());
			EXPECT_VEC(Vec3d(0, -1, 0), q.getRight());
		}

		SECTION("pitch") {
			q = quaternion::eulerAngle<double>(0, 0, 0.5 * pi);
			EXPECT_VEC(Vec3d(0, -1, 0), q.getForward());
			EXPECT_VEC(Vec3d(0, 0, 1), q.getUp());
			EXPECT_VEC(Vec3d(-1, 0, 0), q.getRight());
		}
	}
}
