#include "sceneNode.h"
#include "sceneGroup.h"
#include "sceneObject.h"

namespace gl3d
{
	SceneNode::SceneNode(const NodeType type, const std::string name)
		: type(type), name(name)
	{
	}

	SceneNode::SceneNode()
		: type(NodeType::NODE), name("")
	{
	}

	SceneNode& SceneNode::operator=(const SceneNode& other)
	{
		// TODO check for equal types
		this->name = other.name;
		return *this;
	}

	void SceneNode::updateChildren(
		const std::function<void(SceneObject*, gml::Matrix4D<float>)> func,
		const std::function<gml::Matrix4D<float>(SceneGroup*, gml::Matrix4D<float>)> func2,
		const gml::Matrix4D<float> transform)
	{
		for (SceneNode* child : children)
		{
			switch (child->getType())
			{
			case NodeType::GROUP:
				child->updateChildren(func, func2, func2(static_cast<SceneGroup*>(child), transform));
				break;
			case NodeType::OBJECT:
				func(static_cast<SceneObject*>(child), transform);
				break;
			default: break;
			}
		}
	}

	NodeType SceneNode::getType() const
	{
		return type;
	}

	std::string SceneNode::getName() const
	{
		return name;
	}
}
