###################################################################
# source/core				                                      #
###################################################################

include( ${YAGE_CMAKE_PATH}/sources-core.cmake )

set(YAGE_CORE_COMMON_SOURCE
	${YAGE_PLATFORM_SOURCE}
	${YAGE_GRAPHICS_SOURCE} 
	${YAGE_MATH_SOURCE}
	${YAGE_INPUT_SOURCE}
)
set(YAGE_CORE_ANDROID_SOURCE
	${YAGE_PLATFORM_ANDROID_SOURCE}
	${YAGE_GRAPHICS_GLES2_SOURCE} 
)
set(YAGE_CORE_DESKTOP_SOURCE
	${YAGE_PLATFORM_DESKTOP_SOURCE}
	${YAGE_GRAPHICS_GL3_SOURCE}
)

if(ANDROID)
	add_library(YAGEcore STATIC ${YAGE_CORE_COMMON_SOURCE} ${YAGE_CORE_ANDROID_SOURCE})

	target_link_libraries(YAGEcore libEGL.so
								   libGLESv2.so
								   libandroid.so
								   liblog.so)

	target_include_directories(YAGEcore PUBLIC ${YAGE_CORE_PATH})

	target_compile_definitions(YAGEcore PUBLIC "ANDROID")
	target_compile_definitions(YAGEcore PUBLIC "GLES2")

elseif(WIN32)
	add_library(YAGEcore STATIC ${YAGE_CORE_COMMON_SOURCE} ${YAGE_CORE_DESKTOP_SOURCE})

	target_link_libraries(YAGEcore PUBLIC opengl32.lib
								   PUBLIC glfw
								   PUBLIC glad)

	target_include_directories(YAGEcore PUBLIC ${YAGE_CORE_PATH})

	target_compile_definitions(YAGEcore PUBLIC "DESKTOP")
	target_compile_definitions(YAGEcore PUBLIC "GL3")

elseif(UNIX)
	add_library(YAGEcore STATIC ${YAGE_CORE_COMMON_SOURCE} ${YAGE_CORE_DESKTOP_SOURCE})

	find_package(PkgConfig REQUIRED)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
	# Setup CMake to use GTK+, tell the compiler where to look for headers
	# and to the linker where to look for libraries
	include_directories(${GTK3_INCLUDE_DIRS})
	link_directories(${GTK3_LIBRARY_DIRS})
	# Add other flags to the compiler
	add_definitions(${GTK3_CFLAGS_OTHER})
	
	target_link_libraries(YAGEcore PUBLIC libGL.so
								   PUBLIC ${GTK3_LIBRARIES}
								   PUBLIC glfw
								   PUBLIC glad )

	target_include_directories(YAGEcore PUBLIC ${YAGE_CORE_PATH})

	target_compile_definitions(YAGEcore PUBLIC "DESKTOP")
	target_compile_definitions(YAGEcore PUBLIC "GL3")

endif()