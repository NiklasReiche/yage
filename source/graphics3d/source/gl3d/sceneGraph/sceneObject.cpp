#include "sceneObject.h"

namespace yage::gl3d
{
	SceneObject::SceneObject(const std::string& name, const math::Mat4d transform)
		: SceneNode(name, transform)
	{
	}

    void SceneObject::apply(const std::function<void(SceneObject&)>& f, const math::Mat4d& parent_transform)
    {
        apply_transform(parent_transform);
        f(*this);
    }
}
