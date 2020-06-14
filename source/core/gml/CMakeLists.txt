add_library(yageMath INTERFACE)

target_include_directories(yageMath
	INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(yageMath

	INTERFACE gml/gml.h

	INTERFACE gml/constraints.h
	INTERFACE gml/exception.h

	INTERFACE gml/maths.h
	INTERFACE gml/interpolation.h

	INTERFACE gml/quaternion.h
	INTERFACE gml/quaternion/quaternion.h
	INTERFACE gml/quaternion/quaternionOperations.h
	INTERFACE gml/quaternion/quaternionConstructs.h

	INTERFACE gml/vector.h
	INTERFACE gml/vector/vectorBase.h
	INTERFACE gml/vector/vectorOperations.h
	INTERFACE gml/vector/vec2.h
	INTERFACE gml/vector/vec3.h
	INTERFACE gml/vector/vec4.h

	INTERFACE gml/matrix.h
	INTERFACE gml/matrix/matrixBase.h
	INTERFACE gml/matrix/matrixOperations.h
	INTERFACE gml/matrix/matrixConstructs.h
	INTERFACE gml/matrix/mat2.h
	INTERFACE gml/matrix/mat3.h
	INTERFACE gml/matrix/mat4.h
	)

add_subdirectory(tests)
