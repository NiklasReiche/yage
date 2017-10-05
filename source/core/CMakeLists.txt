#########################
# source/core/Cmake
#########################

include( ${YAGE_CMAKE_PATH}/sources-core.cmake )


if(ANDROID)
	add_library(YAGEcore STATIC ${YAGE_MATH_SOURCE}

								${YAGE_GRAPHICS_SOURCE} 
								${YAGE_GRAPHICS_GLES2_SOURCE} 

								${YAGE_PLATFORM_SOURCE}
								${YAGE_PLATFORM_ANDROID_SOURCE}

								${YAGE_INPUT_SOURCE})

	target_link_libraries(YAGEcore libEGL.so
								   libGLESv2.so
								   libandroid.so
								   liblog.so)

	target_compile_definitions(YAGEcore PUBLIC "ANDROID")
	target_compile_definitions(YAGEcore PUBLIC "GLES2")

elseif(WIN32)
	add_library(YAGEcore STATIC ${YAGE_MATH_SOURCE}

								${YAGE_PLATFORM_SOURCE}
								${YAGE_PLATFORM_DESKTOP_SOURCE}

								${YAGE_INPUT_SOURCE}

								${YAGE_GRAPHICS_SOURCE}
								${YAGE_GRAPHICS_GL3_SOURCE})

	target_link_libraries(YAGEcore PUBLIC opengl32.lib
								   PUBLIC glfw32sMD_d.lib)

	target_compile_definitions(YAGEcore PUBLIC "DESKTOP")
	target_compile_definitions(YAGEcore PUBLIC "GL3")

endif()