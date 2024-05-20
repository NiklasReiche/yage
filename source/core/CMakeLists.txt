message(STATUS "> Configuring yage_core")

add_library(yage_core SHARED)

find_package(Vulkan REQUIRED)

target_link_libraries(yage_core
	PUBLIC yage_gml yage_utils glfw glad ${Vulkan_LIBRARIES} tfd)

target_include_directories(yage_core
	PUBLIC source ${Vulkan_INCLUDE_DIRS})

add_subdirectory(source/core/platform)
add_subdirectory(source/core/gl)
add_subdirectory(source/core/input)

if (YAGE_BUILD_TESTS)
    add_subdirectory(tests)
endif ()


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
