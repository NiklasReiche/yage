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

if(WIN32)
	add_library(YAGEimage STATIC ${image_SRC})

	target_link_libraries(YAGEimage PUBLIC YAGEcore
								    PUBLIC freetype32sMD_d.lib)

endif()