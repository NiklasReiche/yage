###################################################################
# NATIVE FILE DIALOG	                                          #
###################################################################

add_subdirectory(${YAGE_EXTERNAL_PATH}/nfd)
target_include_directories(nfd PUBLIC $<BUILD_INTERFACE:${YAGE_EXTERNAL_PATH}/nfd/include>)