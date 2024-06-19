add_executable(physics_boxes
	main.cpp
	MovementListener.h
	MovementListener.cpp
	app.h
	NewApp.h
	NewApp.cpp
)

target_link_libraries(physics_boxes
	PUBLIC yage_runtime
)
