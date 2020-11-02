add_executable(yage_utils_test
	main.cpp
	stringsTest.cpp
	)

target_link_libraries(yage_utils_test PRIVATE yage_utils Catch2::Catch2)

add_test(
	NAME yage_utils_CTest
	COMMAND yage_utils_test
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})