###################################################################
# /external	                                                      #
###################################################################

if(WIN32 OR UNIX)
    message( STATUS "Building glfw --" )
	include( glfw.cmake )
	message( STATUS "Building glad --" )
	include( glad.cmake )

	message( STATUS "Building zlib --" )
	include( zlib.cmake )
	message( STATUS "Building libpng --" )
	include( libpng.cmake )

	message( STATUS "Building tfd --" )
	include( tfd.cmake )

	message( STATUS "Building gtest --" )
	include( gtest.cmake )
endif()

if(ANDROID)
	message( STATUS "Building zlib --" )
    include( zlib.cmake )
	message( STATUS "Building libpng --" )
	include( libpng.cmake )
endif()

#include(freetype.cmake)