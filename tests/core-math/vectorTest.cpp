#include <gtest/gtest.h>

#include <math/MVector.h>

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(coreTest, vectorTest_Add)
{
    gml::Vec2f vec1(1, 1);
	gml::Vec2f vec2(3, -3);

	gml::Vec2f vec3 = vec1 + vec2;
    EXPECT_EQ(gml::Vec2f(4, -2), vec3);
}
TEST(coreTest, vectorTest_Sub)
{
	gml::Vec2f vec1(1, 1);
	gml::Vec2f vec2(3, -3);

	gml::Vec2f vec3 = vec1 - vec2;
	EXPECT_EQ(gml::Vec2f(-2, 4), vec3);
}
TEST(coreTest, vectorTest_Mul)
{
	gml::Vec2f vec1(1, 1);
	gml::Vec2f vec2(3, -3);

	gml::Vec2f vec3 = vec1 * vec2;
	EXPECT_EQ(gml::Vec2f(3, -3), vec3);

	vec3 = vec2 * 2;
	EXPECT_EQ(gml::Vec2f(6, -6), vec3);
}
TEST(coreTest, vectorTest_Div)
{
	gml::Vec2f vec1(1, 1);
	gml::Vec2f vec2(3, -3);

	gml::Vec2f vec3 = vec2 / 2;
	EXPECT_EQ(gml::Vec2f(1.5f, -1.5f), vec3);
}