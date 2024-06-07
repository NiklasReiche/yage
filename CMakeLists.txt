cmake_minimum_required(VERSION 3.14)

project(yage
	VERSION 0.1
	DESCRIPTION "Yet Another Game Engine"
	LANGUAGES CXX C)

cmake_policy(SET CMP0076 NEW)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

######################
# Global options     #
######################
option(YAGE_BUILD_ANDROID "Cross compile for Android" OFF )
option(YAGE_BUILD_TESTS "Build unit tests" ON )


######################
# Android settings   #
######################
if(YAGE_BUILD_ANDROID)
    set( CMAKE_SYSTEM_NAME Android )
	set( WIN32 0 )
    set( UNIX 0 )
    set( YAGE_BUILD_TESTS FALSE )
endif()


######################
# output directories #
######################
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
	"${CMAKE_SOURCE_DIR}/bin/$<$<BOOL:${WIN32}>:win32>$<$<BOOL:${UNIX}>:unix>/$<$<CONFIG:DEBUG>:debug>$<$<CONFIG:RELEASE>:release>")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	"${CMAKE_SOURCE_DIR}/bin/$<$<BOOL:${WIN32}>:win32>$<$<BOOL:${UNIX}>:unix>/$<$<CONFIG:DEBUG>:debug>$<$<CONFIG:RELEASE>:release>")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
	"${CMAKE_SOURCE_DIR}/bin/$<$<BOOL:${WIN32}>:win32>$<$<BOOL:${UNIX}>:unix>/$<$<CONFIG:DEBUG>:debug>$<$<CONFIG:RELEASE>:release>")


######################
# subdirectories 	 #
######################

if(YAGE_BUILD_TESTS)
	enable_testing()
endif()

set(CMAKE_CXX_FLAGS_OLD "${CMAKE_CXX_FLAGS}")
add_subdirectory(external)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_OLD}")

add_subdirectory(source/utils)
add_subdirectory(source/gml)
add_subdirectory(source/core)
add_subdirectory(source/resource)

if(WIN32 OR UNIX)
	add_subdirectory(source/image)
	add_subdirectory(source/graphics3d)
	add_subdirectory(source/physics3d)
	add_subdirectory(source/font)
	add_subdirectory(source/gui)
endif()

if(ANDROID)
	add_subdirectory( source/tools/yageApp)
elseif(WIN32 OR UNIX)
	add_subdirectory(demo/SimpleWindow)
	add_subdirectory(demo/ImageViewer)
	add_subdirectory(demo/ModelViewer)
	add_subdirectory(demo/physics/billiards)
	add_subdirectory(demo/physics/boxes)
	add_subdirectory(demo/TextRenderer)
	add_subdirectory(demo/GuiDemo)
	
	#add_subdirectory( source/tools/skyboxViewer )
endif()
