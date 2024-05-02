message(STATUS "> Configuring yage_physics3d")

add_library(yage_physics3d SHARED)

target_compile_options(yage_physics3d
	PRIVATE -fdiagnostics-color=always -Wall -Wextra)

target_link_libraries(yage_physics3d
	PUBLIC yage_core yage_gml yage_utils)

target_include_directories(yage_physics3d
	PUBLIC source)

add_subdirectory(source/physics3d)

if(YAGE_BUILD_TESTS)
	add_subdirectory(tests)
endif()