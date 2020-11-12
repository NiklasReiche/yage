#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <gml/matrix.h>


namespace gl3d
{
	class SceneObject;
	class SceneGroup;

	/**
	 * @brief Represents the different node types.
	 */
	enum class NodeType
	{
		NODE,
		GROUP,
		OBJECT,
	};

	/**
	 * @brief Represents a basic node in a scene graph.
	 */
	class SceneNode
	{
	private:
		/** @brief This node's type. Used for class identification. */
		const NodeType type;
		/** @brief This node's name. Used for object identification. */
		std::string name;
		/** @brief This node's local transformation matrix. */
		gml::Mat4d transform;

	protected:
		std::vector<std::shared_ptr<SceneNode>> children;

		/**
		 * @brief Constructs a new node of a specific type with a custom name
		 * and a custom transform.
		 * 
		 * @param type the type
		 * @param name the name
		 * @param transform the transform
		 */
		SceneNode(NodeType type, std::string name, gml::Mat4d transform);

	public:
		/**
		 * @brief Constructs a new node with an empty name and identity transform.
		 */
		SceneNode();

		/**
		 * @brief Assignment operator overload.
		 * 
		 * @param other assign object
		 * @return this
		 */
		SceneNode& operator=(const SceneNode & other);

		/**
		 * @brief Traverses this node's child nodes and calls the given functions.
		 * 
		 * @param func function to handle an object node
		 * @param transform the node's transform
		 */
		void updateChildren(
			std::function<void(SceneObject*, gml::Mat4d)> func,
			gml::Mat4d transform = gml::Mat4d());

		/**
		* @brief Applies the given transform to this node's transform.
		* The matrices are multiplied. This node's transform remains unmodified.
		*
		* @param transform the transform to apply
		* @return the resulting transform
		*/
		gml::Mat4d applyTransform(const gml::Mat4d& transform) const;

		/**
		* @brief Sets this node's transfomation matrix.
		*
		* @param tranform the transform to set
		*/
		void setTransform(const gml::Mat4d& tranform);

		/**
		 * @brief Return's this node's local transform.
		 * 
		 * @return the transformation matrix
		 */
		gml::Mat4d getTransform() const;

		/**
		 * @brief Returns the node's type.
		 * 
		 * @return the type
		 */
		NodeType getType() const;

		/**
		 * @brief Returns this node's custom name.
		 * 
		 * @return the name
		 */
		std::string getName() const;
	};
}
