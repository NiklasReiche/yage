# YAGE Skybox Viewer

set( YAGE_SKYBOX_VIEWER_SOURCE

	"main.cpp"
	"app.h"
	"app.cpp"
	"gui.h"
	"gui.cpp"
	
)

add_executable(skyboxViewer ${YAGE_SKYBOX_VIEWER_SOURCE})

target_link_libraries(skyboxViewer PUBLIC YAGEcore
								   PUBLIC YAGEimage
								   PUBLIC YAGEfont
								   PUBLIC YAGEgui
								   PUBLIC YAGEgraphics3d)