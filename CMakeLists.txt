cmake_minimum_required(VERSION 3.6)

project(YAGE CXX C)
set(CMAKE_CXX_STANDARD 11)

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
set( YAGE_CMAKE_PATH "${CMAKE_SOURCE_DIR}/Cmake"  )

# include directories
include_directories( ${YAGE_CORE_PATH} )
include_directories( ${YAGE_MODULES_PATH} )

if(ANDROID)
	include_directories("${CMAKE_ANDROID_NDK}/sysroot/usr/include")
elseif(WIN32)
	include_directories("external/include")
endif()


# link directories
if(ANDROID)
	link_directories("${CMAKE_ANDROID_NDK}/platforms/android-${CMAKE_ANDROID_API}/arch-${CMAKE_ANDROID_ARCH_ABI}/usr/lib")
elseif(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/external/lib)
	#link_directories(${CMAKE_SOURCE_DIR}/bin/win)
endif()


add_subdirectory(source/core)
add_subdirectory(source/modules)

if(WIN32 AND NOT ANDROID)
	add_subdirectory(tests/graphics)
	add_subdirectory(tests/image)
	add_subdirectory(tests/font)
	add_subdirectory(tests/gui)
	add_subdirectory(tests/graphics3d)
endif()