cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
message(STATUS "> Configuring external dependencies")

include(FetchContent)

# - Catch 2 ------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	catch2

	GIT_REPOSITORY https://github.com/catchorg/Catch2
	GIT_TAG 	   v3.5.3
)
FetchContent_MakeAvailable(catch2)

# - GLAD ---------------------------------------------------------------------------------------------------------------
# Using pre-generated glad files, since adding the Glad generation step to CMake requires a python environment with the
# correct libraries
add_subdirectory(glad) # GL spec: core-4.6

# - GLFW ---------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	glfw

	GIT_REPOSITORY https://github.com/glfw/glfw
	GIT_TAG 3.4
)

set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

FetchContent_MakeAvailable(glfw)

# - zlib ---------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	zlib

	GIT_REPOSITORY https://github.com/madler/zlib
	GIT_TAG v1.3.1
)

option(ZLIB_BUILD_EXAMPLES "Enable Zlib Examples" OFF)

FetchContent_MakeAvailable(zlib)

add_library(ZLIB::ZLIB ALIAS zlib)

# - libpng -------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	libpng

	GIT_REPOSITORY https://github.com/glennrp/libpng
	GIT_TAG v1.6.43
)
option(PNG_TESTS "Build libpng tests" OFF)
option(PNG_DEBUG "Build with debug output" OFF)
option(PNG_TOOLS "Build the libpng tools" OFF)
set(ZLIB_LIBRARY zlib CACHE STRING "zlib library name")
set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
set(SKIP_INSTALL_ALL 1)
FetchContent_MakeAvailable(libpng)
target_include_directories(png_shared INTERFACE
	$<BUILD_INTERFACE:${libpng_SOURCE_DIR}>
	$<BUILD_INTERFACE:${libpng_BINARY_DIR}>)
target_include_directories(png_static INTERFACE
	$<BUILD_INTERFACE:${libpng_SOURCE_DIR}>
	$<BUILD_INTERFACE:${libpng_BINARY_DIR}>)

# - json ---------------------------------------------------------------------------------------------------------------
FetchContent_Declare(json
		GIT_REPOSITORY https://github.com/nlohmann/json
		GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# - tiny_gltf ----------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	tiny_gltf

	GIT_REPOSITORY https://github.com/syoyo/tinygltf
	GIT_TAG v2.8.21
)

option(TINYGLTF_BUILD_LOADER_EXAMPLE "Build loader_example(load glTF and dump infos)" OFF)
option(TINYGLTF_BUILD_GL_EXAMPLES "Build GL exampels(requires glfw, OpenGL, etc)" OFF)
option(TINYGLTF_BUILD_VALIDATOR_EXAMPLE "Build validator exampe" OFF)
option(TINYGLTF_BUILD_BUILDER_EXAMPLE "Build glTF builder example" OFF)
option(TINYGLTF_HEADER_ONLY "On: header-only mode. Off: create tinygltf library(No TINYGLTF_IMPLEMENTATION required in your project)" OFF)
option(TINYGLTF_INSTALL "Install tinygltf files during install step. Usually set to OFF if you include tinygltf through add_subdirectory()" OFF)
option(TINYGLTF_USE_CPP14 "Use C++14 feature(requires C++14 compiler). This may give better performance than C++11" ON)

FetchContent_MakeAvailable(tiny_gltf)

# - TFD ----------------------------------------------------------------------------------------------------------------
add_subdirectory(tfd)
