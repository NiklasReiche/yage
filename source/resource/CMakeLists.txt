message(STATUS "> Configuring yage_resource")

add_library(yage_resource INTERFACE
        source/resource/Store.h
        source/resource/Resource.h
        source/resource/Loader.h)

target_compile_options(yage_resource
        INTERFACE -fdiagnostics-color=always -Wall -Wextra -Werror)

target_include_directories(yage_resource
        INTERFACE source)

add_subdirectory(source/resource)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()