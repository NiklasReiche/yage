#include "sceneObject.h"

namespace graphics3d
{
	SceneObject::SceneObject()
		:objectID(0)
	{
		this->type = NodeType::OBJECT;
	}
	SceneObject::SceneObject(int id)
		:objectID(id)
	{
		this->type = NodeType::OBJECT;
	}

	int SceneObject::getID()
	{
		return objectID;
	}
}