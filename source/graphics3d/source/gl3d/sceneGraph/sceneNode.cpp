#include "sceneNode.h"

namespace gl3d
{
	SceneNode::SceneNode(const std::string_view& name, const gml::Mat4d& transform)
		: local_transform(transform), m_name(name)
	{
	}

	void SceneNode::apply_transform(const gml::Mat4d& parent_transform)
	{
        m_world_transform = parent_transform * local_transform;
	}

	std::string_view SceneNode::name() const
	{
		return m_name;
	}

    gml::Mat4d SceneNode::world_transform() const
    {
        return m_world_transform;
    }
}
