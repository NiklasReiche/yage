#pragma once

#include "graphics/graphics.h"

#include "sceneNode.h"

namespace graphics3d
{
	class SceneObject : public SceneNode
	{
	private:
		int objectID = 0;

 	public:
		SceneObject();
		SceneObject(int id);

		int getID();
	};
}
