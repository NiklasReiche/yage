add_executable(yage_core_test)

target_sources(yage_core_test PRIVATE main.cpp)

target_link_libraries(yage_core_test
	PRIVATE
		yage_core
		Catch2::Catch2
	)

add_subdirectory(gl)
add_subdirectory(platform)

add_test(
	NAME yage_core_CTest
	COMMAND yage_core_test
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
