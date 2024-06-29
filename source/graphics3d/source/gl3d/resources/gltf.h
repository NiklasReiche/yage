#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include <core/gl/TextureCreator.h>

#include "../mesh.h"
#include "../sceneGraph/sceneGroup.h"
#include "../shaders.h"

namespace yage::gl3d::resources
{
    std::vector<SceneGroup> gltf_read_scene(
            const platform::IFileReader& fileReader, const std::string& filename,
            std::unordered_map<int, std::reference_wrapper<SceneObject>>& mesh_nodes);

    std::vector<Mesh> gltf_read_meshes(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            const ShaderMap& shaders);
}
