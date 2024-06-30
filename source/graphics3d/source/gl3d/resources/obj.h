#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include "../material.h"
#include "../mesh.h"
#include "../shaders.h"

namespace yage::gl3d::resources
{
    std::unordered_map<std::string, Material> read_mtl(
        const std::string& filename,
        const platform::IFileReader& file_reader,
        gl::ITextureCreator& texture_creator,
        const ShaderMap& shaders);

    std::vector<Mesh> read_obj(
        const std::string& filename,
        const platform::IFileReader& file_reader,
        gl::ITextureCreator& texture_creator,
        gl::IDrawableCreator& drawable_creator,
        const ShaderMap& shaders);
}
