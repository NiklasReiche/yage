# TODO: find a way to do this at build time
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/pbrShader.vert)
file(READ shaders/pbrShader.vert PBR_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/pbrShader.frag)
file(READ shaders/pbrShader.frag PBR_FRAG)
configure_file(shaders.in.cpp shaders.cpp @ONLY)
configure_file(shaders.h shaders.h) #also copy the header to resolve relative include in cpp file

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

        shaders.h
        ${CMAKE_CURRENT_BINARY_DIR}/shaders.cpp
)