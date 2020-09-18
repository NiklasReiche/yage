###################################################################
# source/core				                                      #
###################################################################

add_subdirectory(gml)
add_subdirectory(utils)
add_subdirectory(platform)
add_subdirectory(gl)

add_library(yage_core SHARED)

target_link_libraries(yage_core
	PUBLIC
		yage_gml
		yage_utils
		yage_platform
		yage_gl
	)


#if (ANDROID)
#	add_library(YAGEcore STATIC ${YAGE_CORE_COMMON_SOURCE} ${YAGE_CORE_ANDROID_SOURCE})

#	target_link_libraries(YAGEcore
#		libEGL.so
#		libGLESv2.so
#		libandroid.so
#		liblog.so
#		yageMath
#		)

#	target_include_directories(YAGEcore PUBLIC ${YAGE_CORE_PATH}
#		${CMAKE_ANDROID_NDK}/sysroot/usr/include)

#	target_compile_definitions(YAGEcore PUBLIC "ANDROID")
#	target_compile_definitions(YAGEcore PUBLIC "GLES2")
# endif()
