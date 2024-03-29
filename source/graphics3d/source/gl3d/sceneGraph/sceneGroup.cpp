#include "sceneGroup.h"
#include "sceneObject.h"

namespace gl3d
{
	SceneGroup::SceneGroup()
		: SceneNode(NodeType::GROUP, "", gml::matrix::Id<double, 4>)
	{
	}

	SceneGroup::SceneGroup(const std::string& name, const gml::Mat4d transform)
		: SceneNode(NodeType::GROUP, name, transform)
	{
	}

	void SceneGroup::addChild(std::shared_ptr<SceneNode> node)
	{
		children.push_back(std::move(node));
	}
}
