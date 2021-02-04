target_sources(yage_gl3d
	PRIVATE
	camera.h
	camera.cpp

	light/light.h
	light/light.cpp
	light/pointLight.h
	light/pointLight.cpp
	light/dirLight.h
	light/dirLight.cpp

	light/pbr/PointLight.h

	material.h
	material.cpp
	mesh.h
	mesh.cpp

	sceneGraph/sceneNode.h
	sceneGraph/sceneNode.cpp
	sceneGraph/sceneGroup.h
	sceneGraph/sceneGroup.cpp
	sceneGraph/sceneObject.h
	sceneGraph/sceneObject.cpp

	sceneRenderer.h
	sceneRenderer.cpp

	resources/obj.h
	resources/obj.cpp
	resources/gltf.h
	resources/gltf.cpp

	#resourceCache.h
	#resourceManager.h
	)