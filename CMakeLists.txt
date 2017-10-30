cmake_minimum_required(VERSION 3.6)

project( YAGE CXX C )

set( YAGE_VERSION_MAJOR 0 )
set( YAGE_VERSION_MINOR 1 )

set( CMAKE_CXX_STANDARD 14 )

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )



######################
# compiler flags     #
######################
set( GCC_COMPILER_FLAGS
	"-fdiagnostics-color=always"
)
set( MSVC_COMPILER_FLAGS
	"/MP"
)

if(WIN32)
	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MSVC_COMPILER_FLAGS}" )
elseif(UNIX)
	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILER_FLAGS}"  )
endif()


######################
# output directories #
######################
if(ANDROID)
	#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/android/${CMAKE_ANDROID_ARCH_ABI})
elseif(WIN32)
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
# ANDROID stuff 	 #
######################
if(ANDROID)
	include_directories("${CMAKE_ANDROID_NDK}/sysroot/usr/include")
	link_directories("${CMAKE_ANDROID_NDK}/platforms/android-${CMAKE_ANDROID_API}/arch-${CMAKE_ANDROID_ARCH_ABI}/usr/lib")
endif()


######################
# subdirectories 	 #
######################
add_subdirectory( external )

add_subdirectory( source/core )

add_subdirectory( source/modules/image )
add_subdirectory( source/modules/font )
add_subdirectory( source/modules/gui )
add_subdirectory( source/modules/graphics3d )

add_subdirectory( source/tools/skyboxViewer )
add_subdirectory( source/tools/guiEditor )

set(YAGE_BUILD_TESTS 1)

if(YAGE_BUILD_TESTS)
	add_subdirectory( tests )
endif()
