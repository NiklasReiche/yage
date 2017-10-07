###################################################################
# source/modules/gui                                              #
###################################################################

include( ${YAGE_CMAKE_PATH}/sources-modules.cmake )

add_library( YAGEgui STATIC ${YAGE_GUI_SOURCE} )

target_link_libraries( YAGEgui PUBLIC YAGEcore
							   PUBLIC YAGEimage
                               PUBLIC YAGEfont )
                              
target_include_directories( YAGEgui PUBLIC ${YAGE_MODULES_PATH} )