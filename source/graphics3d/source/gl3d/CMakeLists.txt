# TODO: find a way to do this at build time
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/pbr_normal_mapping.vert)
file(READ shaders/pbr_normal_mapping.vert PBR_NORMAL_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/pbr_normal_mapping.frag)
file(READ shaders/pbr_normal_mapping.frag PBR_NORMAL_FRAG)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/pbr.vert)
file(READ shaders/pbr.vert PBR_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/pbr.frag)
file(READ shaders/pbr.frag PBR_FRAG)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/skybox.vert)
file(READ shaders/skybox.vert SKYBOX_VERT)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shaders/skybox.frag)
file(READ shaders/skybox.frag SKYBOX_FRAG)

configure_file(shaders.in.cpp shaders.cpp @ONLY)
configure_file(shaders.h shaders.h) # also copy the header to resolve relative include in cpp file

target_sources(yage_gl3d
        PRIVATE
        camera.h
        camera.cpp

        light.h
        light.cpp

        material.h
        material.cpp
        mesh.h
        mesh.cpp
        skybox.h
        skybox.cpp

        sceneGraph/sceneNode.h
        sceneGraph/sceneNode.cpp
        sceneGraph/sceneGroup.h
        sceneGraph/sceneGroup.cpp
        sceneGraph/sceneObject.h
        sceneGraph/sceneObject.cpp

        sceneRenderer.h
        sceneRenderer.cpp
        ProjectionView.h
        ProjectionView.cpp

        resources/obj.h
        resources/obj.cpp
        resources/gltf.h
        resources/gltf.cpp

        MeshLoader.h
        MeshLoader.cpp
        SceneLoader.h
        SceneLoader.cpp

        shaderSnippets.h

        shaders.h
        ${CMAKE_CURRENT_BINARY_DIR}/shaders.cpp
)