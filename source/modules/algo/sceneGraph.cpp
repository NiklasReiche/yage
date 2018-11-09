#include "sceneGraph.h"

namespace graph
{
	SceneGraphNode::SceneGraphNode(const gml::Mat4d& modelTransform)
		: TreeNode<gml::Mat4d, 2>({ modelTransform, gml::Mat4d() })
	{
	}

	gml::Mat4d SceneGraphNode::foldDepthFirst(const gml::Mat4d& accumulator)
	{
		this->worldTransform = accumulator * this->modelTransform;
		//return this->worldTransform;

	}

	void SceneGraphNode::setModelTransform(const gml::Mat4d& transform)
	{
		this->modelTransform = transform;
	}

	gml::Mat4d SceneGraphNode::getModelTransform() const
	{
		return this->modelTransform;
	}

	gml::Mat4d SceneGraphNode::getWorldTransform() const
	{
		return this->worldTransform;
	}
}
