#include "sceneNode.h"
#include "sceneGroup.h"
#include "sceneObject.h"

namespace gl3d
{
	SceneNode::SceneNode(const NodeType type, const std::string name, const gml::Mat4d transform)
		: type(type), name(name), transform(transform)
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
		const std::function<void(SceneObject*, gml::Mat4d)> func,
		const gml::Mat4d transform)
	{
		for (auto child : children)
		{
			switch (child->getType())
			{
			case NodeType::GROUP:
				child->updateChildren(func, child->applyTransform(transform));
				break;
			case NodeType::OBJECT:
				func(static_cast<SceneObject*>(child.get()), child->applyTransform(transform));
				break;
			default: break;
			}
		}
	}

	gml::Mat4d SceneNode::applyTransform(const gml::Mat4d & transform) const
	{
		return transform * this->transform;
	}

	void SceneNode::setTransform(const gml::Mat4d & transform)
	{
		this->transform = transform;
	}

	gml::Mat4d SceneNode::getTransform() const
	{
		return transform;
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
