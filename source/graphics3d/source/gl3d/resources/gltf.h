#pragma once

#include <string>

#include <core/platform/IFileReader.h>
#include <core/gl/DrawableCreator.h>
#include <gl3d/material.h>

namespace gl3d::resources
{
	std::tuple<std::unique_ptr<gl::IDrawable>, gl3d::Material> readGltf(
		const platform::IFileReader& fileReader, const std::string& filename,
		gl::IDrawableCreator& drawableCreator);
}
