# TODO: find a way to do this at build time
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/point.vert)
file(READ shaders/point.vert POINT_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/point.geom)
file(READ shaders/point.geom POINT_GEOM)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/point.frag)
file(READ shaders/point.frag POINT_FRAG)
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

		shaders.h
		${CMAKE_CURRENT_BINARY_DIR}/shaders.cpp
	)