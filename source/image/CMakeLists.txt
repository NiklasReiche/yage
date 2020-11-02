message(STATUS "> Configuring yage_image")

add_library(yage_image)

target_compile_options(yage_image
	PRIVATE -fdiagnostics-color=always -Wall -Wextra -Werror)

target_link_libraries(yage_image
	PUBLIC yage_core zlib)

target_include_directories(yage_image
	PUBLIC source)

if(YAGE_BUILD_TESTS)
	add_subdirectory(tests)
endif()