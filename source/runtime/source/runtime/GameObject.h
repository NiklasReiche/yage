#pragma once

#include <memory>

#include "gl3d/sceneGraph/sceneObject.h"
#include "physics3d/RigidBody.h"

namespace yage
{
    struct GameObject
    {
        std::optional<std::reference_wrapper<gl3d::SceneObject>> scene_node;
        std::optional<physics3d::RigidBodyHandle> rigid_body;
    };
}
