cmake_minimum_required(VERSION 3.14)

project(yage
	VERSION 0.1
	DESCRIPTION "Yet Another Game Engine"
	LANGUAGES CXX C)

cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0076 NEW)

set(CMAKE_CXX_STANDARD 20)


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

add_subdirectory(external)

add_subdirectory(source/gml)
add_subdirectory(source/core)

if(WIN32 OR UNIX)
	#add_subdirectory( source/modules/image )
	#add_subdirectory( source/modules/font )
	#add_subdirectory( source/modules/gui )
	#add_subdirectory( source/modules/graphics3d )
endif()

if(ANDROID)
	add_subdirectory( source/tools/yageApp)
elseif(WIN32 OR UNIX)
	add_subdirectory( source/tools/SimpleWindow )
	#add_subdirectory( source/tools/skyboxViewer )
	#add_subdirectory( source/tools/guiEditor )
	#add_subdirectory( source/tools/sandbox )
endif()
