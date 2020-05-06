add_executable(coreMathTest
	main.cpp
	vectorTest.cpp
	matrixTest.cpp
	quaternionTest.cpp
	mathTest.cpp vec2Test.cpp)

target_link_libraries(coreMathTest
	PUBLIC yageMath
	PUBLIC gtest
	PUBLIC gmock
	)

add_test(
	NAME coreMathCTest
	COMMAND coreMathTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
