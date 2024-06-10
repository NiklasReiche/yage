#pragma once

#include <functional>
#include <string>

#include <math/matrix.h>

namespace yage::gl3d
{
	class SceneObject;

	/**
	 * Represents an interface for composite and leaf nodes in a scene graph. A node contains local translation,
	 * rotation, and scaling components with respect to its parent node.
	 */
	class SceneNode
	{
	public:
        /** This node's transformation matrix in local space. */
        math::Mat4d local_transform;

        virtual ~SceneNode() = default;

        /**
         * Recursively updates this node's world transform by chaining together the transforms from it's parents.
         * Additionally, applies a given function to any leaf nodes encountered. Traversal happens depth-first.
         * @param f A function that gets invoked for any leaf. The argument is the visited leaf.
         * @param parent_transform The matrix that transforms from the parents local space to world space.
         */
        virtual void apply(const std::function<void(SceneObject&)>& f, const math::Mat4d& parent_transform) = 0;

        /**
         * @return This node's current transformation matrix for mapping from local to world space.
         */
        [[nodiscard]] math::Mat4d world_transform() const;

        /**
         * @return This node's name.
         */
        [[nodiscard]] std::string_view name() const;

    protected:
        SceneNode(const std::string_view& name, const math::Mat4d& transform);

        /**
         * Updates this node's world transform by applying a parents accumulated world transform matrix.
         */
        void apply_transform(const math::Mat4d& parent_transform);

    private:
        /**
         * This node's name. Used for object identification.
         */
        const std::string m_name;

        /**
         * The matrix that transforms from this node's local space to world space.
         */
        math::Mat4d m_world_transform;
	};
}
