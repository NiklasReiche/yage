
enable_testing()

add_executable( allTest 
	main.cpp
	core-math/vectorTest.cpp 
	core-math/matrixTest.cpp
	core-math/quaterniontest.cpp
	core-graphics/contextTest.cpp
)

target_link_libraries( allTest PUBLIC YAGEcore
                                PUBLIC gtest
								PUBLIC gmock)

add_test(
	NAME coreTest 
	COMMAND allTest --gtest_catch_exceptions=0
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})