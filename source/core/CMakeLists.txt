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
								   PUBLIC glad
								   PUBLIC nfd )

	target_include_directories(YAGEcore PUBLIC ${YAGE_CORE_PATH})

	target_compile_definitions(YAGEcore PUBLIC "DESKTOP")
	target_compile_definitions(YAGEcore PUBLIC "GL3")

elseif(UNIX)
	add_library(YAGEcore STATIC ${YAGE_CORE_COMMON_SOURCE} ${YAGE_CORE_DESKTOP_SOURCE})
	
	target_link_libraries(YAGEcore PUBLIC libGL.so
								   PUBLIC glfw
								   PUBLIC glad
								   PUBLIC tfd )

	target_include_directories(YAGEcore PUBLIC ${YAGE_CORE_PATH})

	target_compile_definitions(YAGEcore PUBLIC "DESKTOP")
	target_compile_definitions(YAGEcore PUBLIC "GL3")

endif()