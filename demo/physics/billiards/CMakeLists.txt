add_executable(physics_billiards
	"main.cpp"
	"MovementListener.h"
	"MovementListener.cpp"
	"ProjectionView.h"
	"ProjectionView.cpp"
	"app.h" 
)

target_link_libraries(physics_billiards
	PUBLIC yage_core yage_gl3d yage_physics3d
)
