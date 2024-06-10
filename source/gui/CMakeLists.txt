message(STATUS "> Configuring yage_gui")

add_library(yage_gui SHARED)

target_compile_options(yage_gui
        PRIVATE -fdiagnostics-color=always)

target_link_libraries(yage_gui
        PUBLIC yage_core yage_math yage_image yage_font)

target_include_directories(yage_gui
        PUBLIC source)

add_subdirectory(source/gui)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()