cmake_minimum_required(VERSION 3.6)

project( YAGE CXX C )

cmake_policy(SET CMP0076 NEW)

set( YAGE_VERSION_MAJOR 0 )
set( YAGE_VERSION_MINOR 1 )

set( CMAKE_CXX_STANDARD 17 )


######################
# Global options     #
######################
option( YAGE_BUILD_ANDROID "Cross compile for Android" OFF )
option( YAGE_BUILD_TESTS "Build unit tests" ON )


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
# compiler flags     #
######################
set( GCC_COMPILER_FLAGS
		"-fdiagnostics-color=always -Wall -Werror --coverage"
)
set( MSVC_COMPILER_FLAGS
	"/MP"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# using Clang
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# using GCC
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COMPILER_FLAGS}")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# using Visual Studio C++
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSVC_COMPILER_FLAGS}")
endif ()


######################
# output directories #
######################
if(WIN32)
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win )
elseif(UNIX)
	set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux )
	set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux )
	set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux )
endif()


######################
# YAGE paths		 #
######################
set( YAGE_EXTERNAL_PATH "${CMAKE_SOURCE_DIR}/external" 			)
set( YAGE_CMAKE_PATH 	"${CMAKE_SOURCE_DIR}/CMake"  			)
set( YAGE_CORE_PATH 	"${CMAKE_SOURCE_DIR}/source/core" 		)
set( YAGE_MODULES_PATH 	"${CMAKE_SOURCE_DIR}/source/modules" 	)
set( YAGE_TOOLS_PATH 	"${CMAKE_SOURCE_DIR}/source/tools" 		)


######################
# subdirectories 	 #
######################

if (YAGE_BUILD_TESTS)
	enable_testing()
endif ()

add_subdirectory( external )

add_subdirectory( source/core )

if(WIN32 OR UNIX)
	add_subdirectory( source/modules/image )
	add_subdirectory( source/modules/font )
	add_subdirectory( source/modules/gui )
	add_subdirectory( source/modules/graphics3d )
endif()

if(ANDROID)
	add_subdirectory( source/tools/yageApp)
elseif(WIN32 OR UNIX)
	add_subdirectory( source/tools/skyboxViewer )
	add_subdirectory( source/tools/guiEditor )
	add_subdirectory( source/tools/sandbox )
endif()


if(YAGE_BUILD_TESTS)
	enable_testing()
	add_subdirectory( tests )
endif()
