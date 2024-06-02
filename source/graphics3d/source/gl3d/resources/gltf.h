#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include <core/gl/TextureCreator.h>
#include "../mesh.h"
#include "../sceneGraph/sceneGroup.h"

namespace gl3d::resources
{
    std::unique_ptr<gl3d::SceneGroup> gltf_read_scene(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            std::shared_ptr<gl::IShader>& shader,
            std::shared_ptr<gl::IShader>& shader_normal_mapping);

    std::vector<std::shared_ptr<gl3d::Mesh>> gltf_read_meshes(
            const platform::IFileReader& fileReader, const std::string& filename,
            gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
            std::shared_ptr<gl::IShader>& shader,
            std::shared_ptr<gl::IShader>& shader_normal_mapping);
}
