message(STATUS "> Configuring yage_gl3d")

add_library(yage_gl3d SHARED)

target_link_libraries(yage_gl3d
        PUBLIC yage_core yage_utils yage_image nlohmann_json::nlohmann_json tinygltf)

target_include_directories(yage_gl3d
        PUBLIC source)

add_subdirectory(source/gl3d)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()