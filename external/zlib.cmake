###################################################################
# ZLIB	                                                          #
###################################################################

option(BUILD_EXAMPLES "Build usage examples" OFF)

add_subdirectory(${YAGE_EXTERNAL_PATH}/zlib)
target_include_directories(zlibstatic PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/zlib/include>)