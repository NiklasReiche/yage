#include "sceneGroup.h"

namespace gl3d
{
	SceneGroup::SceneGroup()
		: SceneNode(NodeType::GROUP, "")
	{
	}

	SceneGroup::SceneGroup(const std::string name, const gml::Matrix4D<float> transform)
		: SceneNode(NodeType::GROUP, name), transform(transform)
	{
	}

	void SceneGroup::addChild(SceneNode* node)
	{
		children.push_back(node);
	}

	gml::Matrix4D<float> SceneGroup::applyTransform(const gml::Matrix4D<float> transform) const
	{
		return transform * this->transform;
	}

	void SceneGroup::setTransform(const gml::Matrix4D<float> tranform)
	{
		this->transform = tranform;
	}
}
