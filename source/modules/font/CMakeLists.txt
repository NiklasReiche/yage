###################################################################
# source/modules/font		                                      #
###################################################################

include( ${YAGE_CMAKE_PATH}/sources-modules.cmake )

if( YAGE_GUI_CONVERTER )
	add_library( YAGEfont STATIC ${YAGE_FONT_SOURCE} ${YAGE_FONT_CONVERTER_SOURCE} )
	target_link_libraries( YAGEfont PUBLIC freetype )
else()
	add_library( YAGEfont STATIC ${YAGE_FONT_SOURCE} )
endif()

target_link_libraries( YAGEfont PUBLIC YAGEcore )

target_include_directories( YAGEfont PUBLIC ${YAGE_MODULES_PATH} )