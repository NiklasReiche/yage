###################################################################
# tools/sandbox				                                      #
###################################################################

add_executable(sandbox
	"main.cpp"
	"MovementListener.h"
	"ProjectionView.h"
	"ProjectionView.cpp"
	"app.h" 
)

target_link_libraries( sandbox 
	PUBLIC yage_core yage_gl3d yage_physics3d
)
