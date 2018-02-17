#pragma once

#include <vector>
#include <array>
#include <string>

#include <graphics/graphics.h>

namespace gl3d
{
	class Skybox
	{
	private:
		friend class SkyboxLoader;

	public:
		gl::Drawable drawable;
		gl::Texture cubemap;
		gl::Shader shader;
	};

	class SkyboxLoader
	{
	private:
		sys::PlatformHandle* platform;
		gl::GraphicsContext* glContext;

	public:
		SkyboxLoader(sys::PlatformHandle* platform, gl::GraphicsContext* glContext)
			: platform(platform), glContext(glContext){}

		Skybox loadSkybox(std::array<std::string, 6> dirpath, float boxSize, int textureSize);
	};
}