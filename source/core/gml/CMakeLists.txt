add_library(yageMath INTERFACE)

target_include_directories(yageMath
	INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(yageMath

	INTERFACE gml/vector/vectorbase.h
	INTERFACE gml/vector/vectorbase.tpp
	INTERFACE gml/vector/vec2.h
	INTERFACE gml/vector/vec2.tpp
	INTERFACE gml/vector/vec3.h
	INTERFACE gml/vector/vec3.tpp
	INTERFACE gml/vector/vec4.h
	INTERFACE gml/vector/vec4.tpp

	INTERFACE gml/matrix/matrixBase.h
	INTERFACE gml/matrix/matrixBase.tpp
	INTERFACE gml/matrix/matrixSpecial.h
	INTERFACE gml/matrix/mat2.h
	INTERFACE gml/matrix/mat2.tpp
	INTERFACE gml/matrix/mat3.h
	INTERFACE gml/matrix/mat3.tpp
	INTERFACE gml/matrix/mat4.h
	INTERFACE gml/matrix/mat4.tpp

	INTERFACE gml/maths.h
	INTERFACE gml/exception.h
	INTERFACE gml/vector.h
	INTERFACE gml/matrix.h
	INTERFACE gml/quaternion.h
	INTERFACE gml/quaternion.tpp
	INTERFACE gml/interpolation.h
	)

add_subdirectory(tests)
