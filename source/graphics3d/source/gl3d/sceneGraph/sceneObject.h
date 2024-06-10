#pragma once

#include <map>

#include "sceneNode.h"
#include "../mesh.h"
#include "../camera.h"
#include "../light.h"

namespace yage::gl3d
{
	/**
	 * Represents a leaf node in a scene graph. Does not have children, but can reference a mesh, a light, and a camera.
	 */
	class SceneObject : public SceneNode
	{
	public:
        std::shared_ptr<Mesh> mesh = nullptr;
        std::shared_ptr<Light> light = nullptr;
        std::shared_ptr<Camera> camera = nullptr;

        explicit SceneObject(const std::string& name, math::Mat4d transform = math::matrix::Id4d);

        void apply(const std::function<void(SceneObject&)>& f, const math::Mat4d& parent_transform) override;
    };
}
