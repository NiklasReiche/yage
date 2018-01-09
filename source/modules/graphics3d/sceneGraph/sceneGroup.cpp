#include "sceneGroup.h"

namespace graphics3d
{
	SceneGroup::SceneGroup()
	{
		this->type = NodeType::GROUP;
	}
	SceneGroup::SceneGroup(gml::Matrix4D<float> transform)
		:transform(transform)
	{
		this->type = NodeType::GROUP;
	}

	void SceneGroup::addChild(SceneNode* node)
	{
		children.push_back(node);
	}

	gml::Matrix4D<float> SceneGroup::applyTransform(gml::Matrix4D<float> transform)
	{
		return transform * this->transform;
	}

	void SceneGroup::setTransform(gml::Matrix4D<float> tranform)
	{
		this->transform = tranform;
	}
}
