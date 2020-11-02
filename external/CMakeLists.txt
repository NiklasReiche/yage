cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
message(STATUS "> Configuring external dependencies")

include(FetchContent)

# - Catch 2 ------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	catch2

	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG 	   v2.13.1
)
FetchContent_MakeAvailable(catch2)

# - GLAD ---------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	glad

	GIT_REPOSITORY https://github.com/Dav1dde/glad.git
	GIT_TAG v0.1.33
)

set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_API "gl=4.3" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest")
set(GLAD_GENERATOR "c" CACHE STRING "Language to generate the binding for")

FetchContent_MakeAvailable(glad)

# - GLFW ---------------------------------------------------------------------------------------------------------------
FetchContent_Declare(
	glfw

	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.2
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
	GIT_TAG v1.2.11
)
option(BUILD_EXAMPLES OFF)
FetchContent_MakeAvailable(zlib)

# - libpng -------------------------------------------------------------------------------------------------------------
# TODO

# - TFD ----------------------------------------------------------------------------------------------------------------
add_subdirectory(tfd)
