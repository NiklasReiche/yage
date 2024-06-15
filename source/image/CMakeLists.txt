message(STATUS "> Configuring yage_image")

add_library(yage_image)

target_link_libraries(yage_image
        PUBLIC yage_core yage_utils zlib png_shared)

target_include_directories(yage_image
        PUBLIC source)

add_subdirectory(source/image)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()