
add_executable( coreTest 
	main.cpp
	core-math/vectorTest.cpp 
	core-math/matrixTest.cpp
	core-math/quaterniontest.cpp
)

target_link_libraries( coreTest PUBLIC YAGEcore
                                PUBLIC gtest )

add_test(coreTest coreTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})