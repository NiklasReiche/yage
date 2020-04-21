#pragma once

#include "../Cubemap.h"
#include "GL3_Texture.h"

namespace gl3
{
	class GL3_Cubemap : public GL3_Texture, public gl::ICubemap, public std::enable_shared_from_this<GL3_Cubemap>
	{
	public:
		int getWidth() const override;
		int getHeight() const override;
		int getChannels() const override;

	private:
		friend class GL3_Texture_Creator;

		int width = 0;
		int height = 0;
	};
}
