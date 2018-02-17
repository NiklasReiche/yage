#pragma once

#include "math/matrix.h"

#include "sceneNode.h"

namespace gl3d
{
	/**
	 * @brief Represents a group node in a scene graph.
	 * This node type has the ability to add children.
	 */
	class SceneGroup : public SceneNode
	{
	private:
		/** @brief This node's transformation matrix. */
		gml::Matrix4D<float> transform;

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
		SceneGroup(std::string name, gml::Matrix4D<float> transform);

		/**
		 * @brief Adds a node as a child to this node.
		 * 
		 * @param node the node to add
		 */
		void addChild(SceneNode* node);

		/**
		 * @brief Applies the given transform to this node's transform.
		 * The matrices are multiplied. This node's transform remains unmodified.
		 * 
		 * @param transform the transform to apply
		 * @return the resulting transform
		 */
		gml::Matrix4D<float> applyTransform(gml::Matrix4D<float> transform) const;

		/**
		 * @brief Sets this node's transfomation matrix.
		 * 
		 * @param tranform the transform to set
		 */
		void setTransform(gml::Matrix4D<float> tranform);
	};
}
