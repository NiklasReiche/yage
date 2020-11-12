#pragma once

#include <gml/matrix.h>

#include "sceneNode.h"

namespace gl3d
{
	/**
	 * @brief Represents a group node in a scene graph.
	 * This node type has the ability to add children.
	 */
	class SceneGroup : public SceneNode
	{
	public:
		/**
		 * @brief Constructs a group node with an idendity transform and an 
		 * empty name.
		 */
		SceneGroup();

		/**
		 * @brief Construct a group node with a custom name and transform matrix.
		 * 
		 * @param name the name
		 * @param transform the transform
		 */
		explicit SceneGroup(
			std::string name, 
			gml::Mat4d transform = gml::Mat4d());

		/**
		 * @brief Adds a group node as a child to this node.
		 * 
		 * @param node the node to add
		 */
		void addChild(std::shared_ptr<SceneNode> node);
	};
}
