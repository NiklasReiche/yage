#include "sceneObject.h"

namespace gl3d
{
	SceneObject::SceneObject()
		: SceneNode(NodeType::OBJECT, "")
	{
	}

	SceneObject::SceneObject(const std::string name)
		: SceneNode(NodeType::OBJECT, name)
	{
	}

	void SceneObject::addId(SceneObjectIdCode code, int nodeId)
	{
		resourceIds.insert(std::pair<SceneObjectIdCode, int>(code, nodeId));
	}

	bool SceneObject::hasId(const SceneObjectIdCode code) const
	{
		return resourceIds.count(code) > 0;
	}

	int SceneObject::getId(const SceneObjectIdCode code) const
	{
		return resourceIds.at(code);
	}
}
