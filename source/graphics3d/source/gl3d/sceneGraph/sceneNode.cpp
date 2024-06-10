#include "sceneNode.h"

namespace yage::gl3d
{
	SceneNode::SceneNode(const std::string_view& name, const math::Mat4d& transform)
		: local_transform(transform), m_name(name)
	{
	}

	void SceneNode::apply_transform(const math::Mat4d& parent_transform)
	{
        m_world_transform = parent_transform * local_transform;
	}

	std::string_view SceneNode::name() const
	{
		return m_name;
	}

    math::Mat4d SceneNode::world_transform() const
    {
        return m_world_transform;
    }
}
