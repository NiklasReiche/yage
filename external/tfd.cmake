###################################################################
# TINY FILE DIALOGS 	                                          #
###################################################################

add_subdirectory(${YAGE_EXTERNAL_PATH}/tfd)
target_include_directories( tfd PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/tfd/include> )
target_compile_options(tfd
	PRIVATE
	$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
	-w>
	$<$<CXX_COMPILER_ID:MSVC>:
	>)