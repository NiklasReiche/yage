message(STATUS "Configuring yage_gl")

add_library(yage_platform STATIC)

target_compile_options(yage_platform
	PRIVATE -fdiagnostics-color=always -Wall -Wextra -Werror)

target_compile_definitions(yage_platform
	PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

target_include_directories(yage_platform
	PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(yage_platform
	PUBLIC
		yage_utils
		glfw
		glad
		tfd
)

target_sources(yage_platform
	PRIVATE
		platform/File.h
		platform/FileLoader.h
		platform/FileException.h
		platform/OpenGL.h
		platform/Window.h

		platform/input/Event.h
		platform/input/Event.cpp
		platform/input/KeyEvent.h
		platform/input/KeyEvent.cpp
		platform/input/MousePosEvent.h
		platform/input/MousePosEvent.cpp
		platform/input/MouseWheelEvent.h
		platform/input/MouseWheelEvent.cpp
	)

if (WIN32 OR UNIX)
	add_subdirectory(platform/desktop)
elseif(ANDROID)
	add_subdirectory(platform/android)
endif()
