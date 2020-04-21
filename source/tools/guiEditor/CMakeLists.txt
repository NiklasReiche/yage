


set( YAGE_GUI_EDITOR_SOURCE

	"main.cpp"
	
)

add_executable( guiEditor ${YAGE_GUI_EDITOR_SOURCE} )

target_link_libraries( guiEditor PUBLIC YAGEcore
								 PUBLIC YAGEimage
								 PUBLIC YAGEfont
								 PUBLIC YAGEgui )
