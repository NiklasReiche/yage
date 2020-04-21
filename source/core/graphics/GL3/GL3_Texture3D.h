#pragma once

#include "GL3_Texture.h"
#include "../Texture3D.h"

namespace gl3
{
	class GL3_Texture3D : public GL3_Texture, public gl::ITexture3D, public std::enable_shared_from_this<GL3_Texture3D>
	{
	public:

	private:
		int width = 0;
		int height = 0;
		int depth = 0;

		friend class GL3_TextureCreator;
	};
}
