###################################################################
# TINY FILE DIALOGS 	                                          #
###################################################################

add_subdirectory(${YAGE_EXTERNAL_PATH}/tfd)
target_include_directories( tfd PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/tfd/include> )