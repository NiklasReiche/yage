#include <gtest/gtest.h>

#include <gml/maths.h>
#include <gml/interpolation.h>

class MathTest : public testing::Test
{
};

TEST(MathTest, RadDegConversion)
{
	// double implementation
	ASSERT_DOUBLE_EQ(gml::toDeg(0.0), 0.0);
	ASSERT_DOUBLE_EQ(gml::toRad(0.0), 0.0);

	ASSERT_DOUBLE_EQ(gml::toDeg(gml::PI), 180.0);
	ASSERT_DOUBLE_EQ(gml::toDeg(2 * gml::PI), 360.0);
	ASSERT_DOUBLE_EQ(gml::toDeg(3 * gml::PI), 540.0);
	ASSERT_DOUBLE_EQ(gml::toRad(180.0), gml::PI);
	ASSERT_DOUBLE_EQ(gml::toRad(360.0), 2* gml::PI);
	ASSERT_DOUBLE_EQ(gml::toRad(540.0), 3 * gml::PI);

	ASSERT_DOUBLE_EQ(gml::toDeg(-gml::PI), -180.0);
	ASSERT_DOUBLE_EQ(gml::toRad(-180.0), -gml::PI);

	// float implementation
	const float pi_f = static_cast<float>(gml::PI);
	ASSERT_FLOAT_EQ(gml::toDeg(0.0f), 0.0f);
	ASSERT_FLOAT_EQ(gml::toRad(0.0f), 0.0f);

	ASSERT_FLOAT_EQ(gml::toDeg(pi_f), 180.0f);
	ASSERT_FLOAT_EQ(gml::toDeg(2 * pi_f), 360.0f);
	ASSERT_FLOAT_EQ(gml::toDeg(3 * pi_f), 540.0f);
	ASSERT_FLOAT_EQ(gml::toRad(180.0f), pi_f);
	ASSERT_FLOAT_EQ(gml::toRad(360.0f), 2 * pi_f);
	ASSERT_FLOAT_EQ(gml::toRad(540.0f), 3 * pi_f);

	ASSERT_FLOAT_EQ(gml::toDeg(-pi_f), -180.0f);
	ASSERT_FLOAT_EQ(gml::toRad(-180.0f), -pi_f);
}

TEST(MathTest, Lerp)
{
	ASSERT_DOUBLE_EQ(gml::lerp(0.0, 1.0, 0.5), 0.5);
	ASSERT_DOUBLE_EQ(gml::lerp(0.0, 0.0, 0.5), 0);
	ASSERT_DOUBLE_EQ(gml::lerp(0.0, 3.0, 1/3.0), 1);
	ASSERT_DOUBLE_EQ(gml::lerp(-3.0, 1.0, 0.5), -1);

	ASSERT_EQ(gml::lerp(0, 10, 0.5), 5);
}
