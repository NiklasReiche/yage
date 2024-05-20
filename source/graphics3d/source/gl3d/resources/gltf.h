#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include <core/gl/TextureCreator.h>
#include "../mesh.h"
#include "../sceneGraph/sceneGroup.h"

namespace gl3d::resources
{
    std::shared_ptr<gl3d::SceneGroup> readGltf(
		const platform::IFileReader& fileReader, const std::string& filename,
		gl::IDrawableCreator& drawableCreator, gl::ITextureCreator& textureCreator,
        std::shared_ptr<gl::IShader>& shader);
}
