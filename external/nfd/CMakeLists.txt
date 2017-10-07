

set( NFD_COMMON_SOURCE
    "nfd.h"    
    "common.h"
    "nfd_common.h"
    "nfd_common.c"
)

if(WIN32)
    set( NFD_SOURCE ${NFD_COMMON_SOURCE} "nfd_win.cpp" )
elseif(UNIX)
    set( NFD_SOURCE ${NFD_COMMON_SOURCE} "nfd_gtk.c" )
endif()

add_library( nfd STATIC ${NFD_SOURCE} )

if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

    link_directories(${GTK3_LIBRARY_DIRS})
    target_link_libraries( nfd PUBLIC ${GTK3_LIBRARIES} )

    target_include_directories( nfd PUBLIC ${GTK3_INCLUDE_DIRS} )
    add_definitions( ${GTK3_CFLAGS_OTHER} )
endif()
