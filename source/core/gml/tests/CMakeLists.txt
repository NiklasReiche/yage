add_executable(coreMathTest
	main.cpp
	mathTest.cpp
	vectorTest.cpp
	vecXTest.cpp
	matrixTest.cpp
	matXTest.cpp
	quaternionTest.cpp)

target_link_libraries(coreMathTest
	PUBLIC yageMath
	PUBLIC catch2
	)

add_test(
	NAME coreMathCTest
	COMMAND coreMathTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
