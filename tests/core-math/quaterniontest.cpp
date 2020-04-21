#include <gtest/gtest.h>

#include <math/quaternion.h>
#include "math/maths.h"

using namespace gml;

class QuatenionTest : public testing::Test
{
protected:
	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

template<typename T>
static void EXPECT_QUAT(Quaternion<T> expected, Quaternion<T> result)
{
	EXPECT_NEAR(expected.w, result.w, 0.0001);
	EXPECT_NEAR(expected.x, result.x, 0.0001);
	EXPECT_NEAR(expected.y, result.y, 0.0001);
	EXPECT_NEAR(expected.z, result.z, 0.0001);
}
template<typename T, size_t size>
static void EXPECT_VEC(Vector<T, size> expected, VectorBase<T, size> result)
{
	for (auto i = 0; i < size; ++i)
	{
		EXPECT_NEAR(expected.at(i), result.at(i), 0.0001);
	}
}

TEST(QuaternionTest, Euler)
{
	Quatd quat = Quatd::eulerAngle(0, 0, 0);
	EXPECT_QUAT(Quatd(1, 0, 0, 0), quat);

	quat = Quatd::eulerAngle(0.5*PI, 0, 0);
	EXPECT_QUAT(Quatd(0.7071, 0, 0.7071, 0), quat);
	EXPECT_NEAR(0.5*PI, quat.getYaw(), 0.0001);
	EXPECT_NEAR(0, quat.getRoll(), 0.0001);
	EXPECT_NEAR(0, quat.getPitch(), 0.0001);

	quat = Quatd::eulerAngle(0, 0.5*PI, 0);
	EXPECT_QUAT(Quatd(0.7071, 0, 0, 0.7071), quat);
	EXPECT_NEAR(0, quat.getYaw(), 0.0001);
	EXPECT_NEAR(0.5*PI, quat.getRoll(), 0.0001);
	EXPECT_NEAR(0, quat.getPitch(), 0.0001);

	quat = Quatd::eulerAngle(0, 0, 0.5*PI);
	EXPECT_QUAT(Quatd(0.7071, 0.7071, 0, 0), quat);
	EXPECT_NEAR(0, quat.getYaw(), 0.0001);
	EXPECT_NEAR(0, quat.getRoll(), 0.0001);
	EXPECT_NEAR(0.5*PI, quat.getPitch(), 0.0001);
}

TEST(QuaternionTest, Direction)
{
	Quatd quat = Quatd::eulerAngle(0, 0, 0);
	EXPECT_VEC(Vec3d(0, 0, 1), quat.getForward());
	EXPECT_VEC(Vec3d(0, 1, 0), quat.getUp());
	EXPECT_VEC(Vec3d(-1, 0, 0), quat.getRight());

	quat = Quatd::eulerAngle(0.5*PI, 0, 0);
	EXPECT_VEC(Vec3d(1, 0, 0), quat.getForward());
	EXPECT_VEC(Vec3d(0, 1, 0), quat.getUp());
	EXPECT_VEC(Vec3d(0, 0, 1), quat.getRight());

	quat = Quatd::eulerAngle(0, 0.5*PI, 0);
	EXPECT_VEC(Vec3d(0, 0, 1), quat.getForward());
	EXPECT_VEC(Vec3d(-1, 0, 0), quat.getUp());
	EXPECT_VEC(Vec3d(0, -1, 0), quat.getRight());

	quat = Quatd::eulerAngle(0, 0, 0.5*PI);
	EXPECT_VEC(Vec3d(0, -1, 0), quat.getForward());
	EXPECT_VEC(Vec3d(0, 0, 1), quat.getUp());
	EXPECT_VEC(Vec3d(-1, 0, 0), quat.getRight());
}

TEST(QuaternionTest, Conversion)
{
	for (float i = 0; i < 2* PI; i += 0.1)
	{
		gml::Quatd quat = gml::Quatd::axisAngle(gml::Vec3d(0.0, 0.0, 1.0), i);
		gml::Mat4d mat = gml::Mat4d::axisAngle(gml::Vec3d(0.0, 0.0, 1.0), i);

		gml::Quatd quat2 = mat.getRotation().toQuaternion();

		gml::Vec4d vec = mat * gml::Vec4d(0, 1, 0, 0);
		EXPECT_VEC(Vec3d(vec.x, vec.y, vec.z), quat.getUp());
	}
}
