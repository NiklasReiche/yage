###################################################################
# source/modules/image                                            #
###################################################################

include( ${YAGE_CMAKE_PATH}/sources-modules.cmake )

add_library( YAGEimage STATIC ${YAGE_IMAGE_SOURCE} )

target_link_libraries( YAGEimage PUBLIC YAGEcore
								 PUBLIC zlibstatic
                                 PUBLIC png_static )
                                
target_include_directories( YAGEimage PUBLIC ${YAGE_MODULES_PATH} )