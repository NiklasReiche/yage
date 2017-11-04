###################################################################
# /external	                                                      #
###################################################################

if(WIN32 OR UNIX)
	include( glfw.cmake )
	include( glad.cmake )

	include( zlib.cmake )
	include( libpng.cmake )

	include( tfd.cmake )

	include( gtest.cmake )
endif()

if(ANDROID)
    include( zlib.cmake )
	include( libpng.cmake )
endif()

#include(freetype.cmake)