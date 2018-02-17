
add_executable( coreTest core-math/vectorTest.cpp )

target_link_libraries( coreTest PUBLIC YAGEcore
                                PUBLIC gtest )

add_test(coreTest coreTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})