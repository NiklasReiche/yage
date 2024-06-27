message(STATUS "> Configuring yage_runtime")

add_library(yage_runtime SHARED)

target_link_libraries(yage_runtime
        PUBLIC yage_core yage_resource yage_gl3d yage_physics3d)

target_include_directories(yage_runtime
        PUBLIC source)

add_subdirectory(source/runtime)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()