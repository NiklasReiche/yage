#include "sceneObject.h"

namespace gl3d
{
	SceneObject::SceneObject(const std::string& name, const gml::Mat4d transform)
		: SceneNode(name, transform)
	{
	}

    void SceneObject::apply(const std::function<void(SceneObject&)>& f, const gml::Mat4d& parent_transform)
    {
        apply_transform(parent_transform);
        f(*this);
    }
}
