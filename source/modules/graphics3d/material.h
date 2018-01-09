#pragma once

#include <map>
#include <string>

#include "graphics/graphics.h"

namespace graphics3d
{
	class Material
	{
	private:
		gl::Shader shader;
		std::map<std::string, gl::Texture> textures;

	public:
		Material(){}

		void addTexture(gl::Texture tex, std::string name)
		{
			textures[name] = tex;
		}
	};
}