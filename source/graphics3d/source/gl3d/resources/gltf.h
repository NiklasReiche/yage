#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include <core/gl/TextureCreator.h>
#include "../mesh.h"

namespace gl3d::resources
{
    std::unique_ptr<Mesh> readGltf(
		const platform::IFileReader& fileReader, const std::string& filename,
		gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator);
}
