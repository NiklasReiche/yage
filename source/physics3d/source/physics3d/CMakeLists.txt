# TODO: find a way to do this at build time
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/empty.vert)
file(READ shaders/empty.vert EMPTY_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/pass_through.frag)
file(READ shaders/pass_through.frag PASS_THROUGH_FRAG)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/point.geom)
file(READ shaders/point.geom POINT_GEOM)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/vector.geom)
file(READ shaders/vector.geom VECTOR_GEOM)
configure_file(shaders.in.cpp shaders.cpp @ONLY)
configure_file(shaders.h shaders.h) #also copy the header to resolve relative include in cpp file

target_sources(yage_physics3d
	PRIVATE
		RigidBody.h
		RigidBody.cpp
		Simulation.h
		Simulation.cpp
		InertiaShape.h
		InertiaShape.cpp
		BoundingShape.h
		BoundingShape.cpp
		Collision.h
		Algorithms.h
		Algorithms.cpp
		Visualizer.h
		Visualizer.cpp

		shaders.h
		${CMAKE_CURRENT_BINARY_DIR}/shaders.cpp
	)