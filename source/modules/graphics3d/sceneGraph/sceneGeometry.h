#pragma once

#include "graphics/graphics.h"

#include "sceneNode.h"

namespace graphics3d
{
	struct SceneGeometry : SceneNode
	{
	public:
		gl::Drawable drawable;

		SceneGeometry()
		{
			type = 2;
		}
	};
}