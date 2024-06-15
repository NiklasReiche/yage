message(STATUS "> Configuring yage_math")

add_library(yage_math INTERFACE)

target_include_directories(yage_math INTERFACE include)

add_subdirectory(include/math)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()