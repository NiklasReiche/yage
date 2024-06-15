message(STATUS "> Configuring yage_utils")

add_library(yage_utils INTERFACE)

target_include_directories(yage_utils INTERFACE include)

add_subdirectory(include/utils)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()
