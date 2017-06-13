#########################
# source/modules/Cmake
#########################

file(GLOB image_SRC
    "image/*.cpp"
	"image/*.h"
)
file(GLOB font_SRC
	"font/*.cpp"
	"font/*.h"
)
file(GLOB gui_SRC
	"gui/*.cpp"
	"gui/*.h"
)

if(ANDROID)
	add_library(YAGEimage STATIC ${image_SRC})

	target_link_libraries(YAGEimage PUBLIC YAGEcore)


	add_library(YAGEfont STATIC ${font_SRC})

	target_link_libraries(YAGEfont PUBLIC YAGEcore
								   PUBLIC freetype32sMD_d.lib)

	target_compile_definitions(YAGEfont PUBLIC "GLES2")


	add_library(YAGEgui STATIC ${gui_SRC})

	target_link_libraries(YAGEgui PUBLIC YAGEcore
								  PUBLIC YAGEimage
								  PUBLIC YAGEfont)

	target_compile_definitions(YAGEgui PUBLIC "GLES2")

elseif(WIN32)
	add_library(YAGEimage STATIC ${image_SRC})

	target_link_libraries(YAGEimage PUBLIC YAGEcore)


	add_library(YAGEfont STATIC ${font_SRC})

	target_link_libraries(YAGEfont PUBLIC YAGEcore
								   PUBLIC freetype32sMD_d.lib)

	target_compile_definitions(YAGEfont PUBLIC "GL3")


	add_library(YAGEgui STATIC ${gui_SRC})

	target_link_libraries(YAGEgui PUBLIC YAGEcore
								  PUBLIC YAGEimage
								  PUBLIC YAGEfont)

	target_compile_definitions(YAGEgui PUBLIC "GL3")

endif()