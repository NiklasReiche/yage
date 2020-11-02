###################################################################
# ZLIB	                                                          #
###################################################################

option(BUILD_EXAMPLES "Build usage examples" OFF)

add_subdirectory(${YAGE_EXTERNAL_PATH}/zlib)
target_include_directories(zlibstatic PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/zlib/include>)
target_compile_options(zlib
	PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
	-w>
	$<$<CXX_COMPILER_ID:MSVC>:
	>)