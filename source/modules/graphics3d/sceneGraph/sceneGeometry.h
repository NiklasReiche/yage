#pragma once

#include "graphics/graphics.h"

#include "sceneNode.h"

namespace graphics3d
{
	class SceneGeometry : public SceneNode
	{
	public:
		gl::Drawable drawable;

 	public:
		SceneGeometry()
		{
			this->type = 2;
		}
		SceneGeometry(gl::Drawable drawable)
			:drawable(drawable) 
		{
			this->type = 2;
		}

	};
}
