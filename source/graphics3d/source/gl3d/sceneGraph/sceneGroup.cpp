#include "sceneGroup.h"
#include "sceneObject.h"

namespace yage::gl3d
{
	SceneGroup::SceneGroup(const std::string_view& name, const math::Mat4d& transform)
		: SceneNode(name, transform)
	{
	}

    void SceneGroup::apply(const std::function<void(SceneObject&)>& f, const math::Mat4d& parent_transform)
    {
        apply_transform(parent_transform);
        for (auto& child : m_children) {
            child->apply(f, world_transform());
        }
    }
}
