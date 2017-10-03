#include <gtest/gtest.h>
#include <math/Interpolation.h>


int main(int ac, char* av[])
{
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}


TEST(testVector, myVectorTest) 
{
	EXPECT_EQ(5, gml::lerp(1, 10, 0.5));
}