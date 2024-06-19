#pragma once

#include <string>

#include <core/gl/graphics.h>
#include <core/platform/IFileReader.h>

namespace yage::gl3d
{
	struct Skybox
	{
		std::unique_ptr<gl::IDrawable> drawable;
		std::unique_ptr<gl::ICubemap> cubemap;
		std::unique_ptr<gl::IShader> shader;
	};

	Skybox load_skybox(platform::IFileReader& file_reader, gl::IContext& context, std::span<std::string, 6> paths, float box_size, int texture_size);
}