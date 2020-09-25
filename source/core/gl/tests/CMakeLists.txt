add_executable(yage_gl_test
	main.cpp
	contextTest.cpp
	textureTest.cpp
	cubemapTest.cpp
	)

target_link_libraries(yage_gl_test
	PRIVATE
		yage_gl
		Catch2::Catch2
	)

add_test(
	NAME yage_gl_CTest
	COMMAND yage_gl_test
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
