#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include <core/gl/TextureCreator.h>
#include "../mesh.h"
#include "../sceneGraph/sceneGroup.h"
#include "gl3d/shaders.h"

namespace yage::gl3d::resources
{
    std::unique_ptr<gl3d::SceneGroup> gltf_read_scene(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            const ShaderMap& shaders);

    std::vector<std::unique_ptr<gl3d::Mesh>> gltf_read_meshes(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            const ShaderMap& shaders);
}
