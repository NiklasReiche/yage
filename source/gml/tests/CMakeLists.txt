add_executable(yage_gml_test
	main.cpp
	mathTest.cpp
	vectorTest.cpp
	matrixTest.cpp
	quaternionTest.cpp)

target_link_libraries(yage_gml_test
	PRIVATE
		yage_gml
		Catch2::Catch2
	)

add_test(
	NAME yage_gml_CTest
	COMMAND yage_gml_test
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
