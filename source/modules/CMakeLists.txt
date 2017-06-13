#########################
# source/modules/Cmake
#########################

include( ${YAGE_CMAKE_PATH}/sources-modules.cmake )


if(ANDROID)
	add_library(YAGEimage STATIC ${YAGE_IMAGE_SOURCE})

	target_link_libraries(YAGEimage PUBLIC YAGEcore)


	add_library(YAGEfont STATIC ${YAGE_FONT_SOURCE})

	target_link_libraries(YAGEfont PUBLIC YAGEcore
								   )

	#target_compile_definitions(YAGEfont PUBLIC "GLES2")


	add_library(YAGEgui STATIC ${YAGE_GUI_SOURCE})

	target_link_libraries(YAGEgui PUBLIC YAGEcore
								  PUBLIC YAGEimage
								  PUBLIC YAGEfont)

	#target_compile_definitions(YAGEgui PUBLIC "GLES2")

elseif(WIN32)
	add_library(YAGEimage STATIC ${YAGE_IMAGE_SOURCE})

	target_link_libraries(YAGEimage PUBLIC YAGEcore)


	add_library(YAGEfont STATIC ${YAGE_FONT_SOURCE})

	if( YAGE_GUI_CONVERTER )
		set(YAGEfont-linklibs YAGEcore YAGEimage freetype32sMD_d.lib)
	else()
		set(YAGEfont-linklibs YAGEcore)
	endif()
	target_link_libraries( YAGEfont PUBLIC ${YAGEfont-linklibs} )

	#target_compile_definitions(YAGEfont PUBLIC "GL3")


	add_library(YAGEgui STATIC ${YAGE_GUI_SOURCE})

	target_link_libraries(YAGEgui PUBLIC YAGEcore
								  PUBLIC YAGEimage
								  PUBLIC YAGEfont)

	#target_compile_definitions(YAGEgui PUBLIC "GL3")

endif()