add_executable(coreGlTest
	main.cpp
	contextTest.cpp
	textureTest.cpp
	cubemapTest.cpp
	)

target_link_libraries(coreGlTest
	PUBLIC yage_gl
	PUBLIC catch2
	)

add_test(
	NAME coreGlCTest
	COMMAND coreGlTest
	WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
