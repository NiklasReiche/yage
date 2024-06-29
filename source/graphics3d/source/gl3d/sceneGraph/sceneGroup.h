#pragma once

#include <memory>
#include <vector>

#include "sceneNode.h"
#include "sceneObject.h"

namespace yage::gl3d
{
	/**
	 * Represents a composite node in a scene graph. Contains children and delegates all work to them.
	 */
	class SceneGroup final : public SceneNode
	{
	public:
        explicit SceneGroup(const std::string_view& name, const math::Mat4d& transform = math::matrix::Id4d);

		SceneGroup(SceneGroup& other) = delete;

		SceneGroup(SceneGroup&& other) = default;

		SceneGroup& operator=(SceneGroup& other) = delete;

		SceneGroup& operator=(SceneGroup&& other) noexcept
		{
			m_children = std::move(other.m_children);
			return *this;
		}

        void apply(const std::function<void(SceneObject&)>& f, const math::Mat4d& parent_transform) override;

        /**
         * Creates a new group node as a child of this group node.
         * @param args Constructor arguments of the group node.
         * @return A reference to the created node.
         */
        SceneGroup& create_group(const auto&... args)
        {
            auto ptr = std::make_unique<SceneGroup>(args...);
            SceneGroup* raw_ptr = ptr.get();
            m_children.push_back(std::move(ptr));
            return *raw_ptr;
        }

        /**
         * Creates a new leaf node as a child of this group node.
         * @param args Constructor arguments of the leaf node.
         * @return A reference to the created node.
         */
        SceneObject& create_object(const auto&... args)
        {
            auto ptr = std::make_unique<SceneObject>(args...);
            SceneObject* raw_ptr = ptr.get();
            m_children.push_back(std::move(ptr));
            return *raw_ptr;
        }

        /**
         * Adds an existing node to this group. Takes ownership of the node.
         */
        SceneNode& add_node(std::unique_ptr<SceneNode> node)
        {
            m_children.emplace_back(std::move(node));
            return *m_children.back();
        }

    private:
        std::vector<std::unique_ptr<SceneNode>> m_children;
	};
}
