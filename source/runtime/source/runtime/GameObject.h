#pragma once

#include <memory>

#include "gl3d/sceneGraph/sceneObject.h"
#include "physics3d/BoundingShape.h"

namespace yage
{
    struct GameObject
    {
        std::optional<std::reference_wrapper<gl3d::SceneObject>> scene_node;
        std::shared_ptr<physics3d::RigidBody> rigid_body;
    };
}
