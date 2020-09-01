message(STATUS "Configuring yage_gl")

add_library(yage_platform)

target_include_directories(yage_platform
	INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(yage_platform
	PUBLIC yage_utils
	PUBLIC glfw
	PUBLIC glad
	PUBLIC tfd
)

target_sources(yage_platform
	INTERFACE platform/OpenGL.h
	INTERFACE platform/Window.h
	INTERFACE platform/File.h
	INTERFACE platform/FileLoader.h
	INTERFACE platform/FileException.h
	)

if (WIN32 OR UNIX)
	add_subdirectory(platform/desktop)
elseif(ANDROID)
	add_subdirectory(platform/android)
endif()
