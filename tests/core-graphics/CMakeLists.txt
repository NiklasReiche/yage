add_executable(coreGraphicsTest
	../main.cpp
	contextTest.cpp
	textureTest.cpp
	)

target_link_libraries(coreGraphicsTest
	PUBLIC YAGEcore
	PUBLIC gtest
	PUBLIC gmock
	)

add_test(
	NAME coreGraphicsTest
	COMMAND coreGraphicsTest --gtest_catch_exceptions=0
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})