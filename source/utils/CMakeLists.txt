message(STATUS "> Configuring yage_utils")

add_library(yage_utils INTERFACE)

target_compile_options(yage_utils
	INTERFACE -fdiagnostics-color=always -Wall -Wextra)

target_include_directories(yage_utils INTERFACE include)

add_subdirectory(include/utils)
add_subdirectory(tests)
