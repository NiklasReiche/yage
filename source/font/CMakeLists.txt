message(STATUS "> Configuring yage_font")

add_library(yage_font SHARED)

target_link_libraries(yage_font
        PUBLIC yage_core yage_resource yage_math yage_image freetype)

target_include_directories(yage_font
        PUBLIC source)

add_subdirectory(source/font)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()