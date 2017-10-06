cmake_minimum_required(VERSION 3.6)

project(YAGE CXX C)
set(CMAKE_CXX_STANDARD 14)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Gm")

# output directories
if(ANDROID)
	#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/android/${CMAKE_ANDROID_ARCH_ABI})
elseif(WIN32)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win)
endif()


set( YAGE_CORE_PATH "${CMAKE_SOURCE_DIR}/source/core" )
set( YAGE_MODULES_PATH "${CMAKE_SOURCE_DIR}/source/modules" )
set( YAGE_TOOLS_PATH "${CMAKE_SOURCE_DIR}/source/tools" )
set( YAGE_EXTERNAL_PATH "${CMAKE_SOURCE_DIR}/external" )
set( YAGE_CMAKE_PATH "${CMAKE_SOURCE_DIR}/Cmake"  )

# include directories
include_directories( ${YAGE_MODULES_PATH} )


if(ANDROID)
	include_directories("${CMAKE_ANDROID_NDK}/sysroot/usr/include")
	link_directories("${CMAKE_ANDROID_NDK}/platforms/android-${CMAKE_ANDROID_API}/arch-${CMAKE_ANDROID_ARCH_ABI}/usr/lib")
endif()


add_subdirectory(external)

add_subdirectory(source/core)
add_subdirectory(source/modules)

if(WIN32 AND NOT ANDROID)
	add_subdirectory(tests/graphics)
	add_subdirectory(tests/image)
	add_subdirectory(tests/gui)
	add_subdirectory(tests/graphics3d)

	add_subdirectory(source/tools/skyboxViewer)
endif()