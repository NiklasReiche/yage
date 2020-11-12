#pragma once

#include <array>
#include <string>

#include <core/gl/graphics.h>

namespace gl3d
{
	class Skybox
	{
	private:
		friend class SkyboxLoader;

	public:
		gl::Drawable drawable;
		gl::Cubemap cubemap;
		gl::Shader shader;
	};

	class SkyboxLoader
	{
	private:
		gl::Context glContext;

	public:
		SkyboxLoader(gl::Context glContext)
			: glContext(glContext){}

		Skybox loadSkybox(std::array<std::string, 6> dirpath, float boxSize, int textureSize);
	};
}