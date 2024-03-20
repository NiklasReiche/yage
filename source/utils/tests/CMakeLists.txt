add_executable(yage_utils_test
        stringsTest.cpp
	)

target_link_libraries(yage_utils_test PRIVATE yage_utils Catch2::Catch2WithMain)

add_test(
	NAME yage_utils_CTest
	COMMAND yage_utils_test
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})