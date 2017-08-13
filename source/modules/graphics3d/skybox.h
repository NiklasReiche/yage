#pragma once

#include <vector>
#include <array>
#include <string>

#include <graphics/gl.h>
#include <image/img.h>

namespace graphics3d
{
	class Skybox
	{
	private:
		friend class SkyboxLoader;

		gl::Drawable drawable;
		gl::Texture cubemap;
	};

	class SkyboxLoader
	{
	private:
		sys::PlatformHandle* platform;
		gl::GraphicsContext* glContext;

	public:
		SkyboxLoader(sys::PlatformHandle* platform, gl::GraphicsContext* glContext)
			: platform(platform), glContext(glContext){}

		Skybox loadSkybox(std::string dirpath, float boxSize);
	};
}