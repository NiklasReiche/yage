#include <gtest/gtest.h>

#include <math/MVector.h>

int main(int argc, char **argv) 
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(coreTest, vectorTest)
{
    gml::Vec2f vector;
    EXPECT_EQ(0, vector.x);
    EXPECT_EQ(0, vector.y);
}