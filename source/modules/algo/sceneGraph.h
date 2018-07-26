#pragma once
#include "tree/treeNode.h"
#include <math/matrix.h>

namespace graph
{
	class SceneGraphNode : TreeNode<gml::Mat4d, 2>
	{
	private:
		gml::Mat4d& modelTransform = keys.at(0);
		gml::Mat4d& worldTransform = keys.at(1);

	protected:
		SceneGraphNode() = default;
		SceneGraphNode(const gml::Mat4d& modelTransform);

		gml::Mat4d foldDepthFirst(const gml::Mat4d& accumulator) override;

		void setTransform(const gml::Mat4d& transform);
		gml::Mat4d getTransform() const;
	};
}
