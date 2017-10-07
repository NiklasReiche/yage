###################################################################
# LIBPNG                                                          #
###################################################################

option(PNG_BUILD_ZLIB "Custom zlib Location, else find_package is used" ON)

option(PNG_SHARED "Build shared lib" OFF)
option(PNG_STATIC "Build static lib" ON)
option(PNG_TESTS  "Build libpng tests" OFF)

option(PNG_FRAMEWORK "Build OS X framework" OFF)
option(PNG_DEBUG     "Build with debug output" OFF)
option(PNGARG        "Disable ANSI-C prototypes" OFF)

option(PNG_HARDWARE_OPTIMIZATIONS "Enable Hardware Optimizations" ON)

set(ZLIB_LIBRARY "zlib")
set(ZLIB_INCLUDE_DIR ${YAGE_EXTERNAL_PATH}/zlib/include/zlib)
set(PNG_CUSTOM_EXPORT 0)

add_subdirectory(${YAGE_EXTERNAL_PATH}/libpng)
target_include_directories(png_static PUBLIC zlib 
									  PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/libpng/include>)