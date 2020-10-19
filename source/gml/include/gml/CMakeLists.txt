target_sources(yage_gml INTERFACE
	gml.h

	constraints.h
	exception.h

	maths.h
	interpolation.h

	quaternion.h
	quaternion/quaternion.h
	quaternion/quaternionOperations.h
	quaternion/quaternionConstructs.h

	vector.h
	vector/vectorBase.h
	vector/vectorOperations.h
	vector/vec2.h
	vector/vec3.h
	vector/vec4.h

	matrix.h
	matrix/matrixBase.h
	matrix/matrixOperations.h
	matrix/matrixConstructs.h
	matrix/mat2.h
	matrix/mat3.h
	matrix/mat4.h
	)