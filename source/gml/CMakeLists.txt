message(STATUS "> Configuring yage_gml")

add_library(yage_gml INTERFACE)

target_compile_options(yage_gml
	INTERFACE -fdiagnostics-color=always -Wall -Wextra -Werror)

target_include_directories(yage_gml INTERFACE include)

add_subdirectory(include/gml)

add_subdirectory(tests)
