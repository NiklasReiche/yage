message(STATUS "Configuring yage_gml")

add_library(yage_gml INTERFACE)

target_include_directories(yage_gml
	INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(yage_gml
	INTERFACE
		gml/gml.h

		gml/constraints.h
		gml/exception.h

		gml/maths.h
		gml/interpolation.h

		gml/quaternion.h
		gml/quaternion/quaternion.h
		gml/quaternion/quaternionOperations.h
		gml/quaternion/quaternionConstructs.h

		gml/vector.h
		gml/vector/vectorBase.h
		gml/vector/vectorOperations.h
		gml/vector/vec2.h
		gml/vector/vec3.h
		gml/vector/vec4.h

		gml/matrix.h
		gml/matrix/matrixBase.h
		gml/matrix/matrixOperations.h
		gml/matrix/matrixConstructs.h
		gml/matrix/mat2.h
		gml/matrix/mat3.h
		gml/matrix/mat4.h
	)

add_subdirectory(tests)
