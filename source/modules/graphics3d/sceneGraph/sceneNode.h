#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "math/matrix.h"


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

	protected:
		std::vector<SceneNode*> children;

		/**
		 * @brief Constructs a new node of a specific type and a custom name.
		 * 
		 * @param type the type
		 * @param name the name
		 */
		SceneNode(NodeType type, std::string name);

	public:
		/**
		 * @brief Constructs a new node with an empty name.
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
		 * @param func function to handle a group node
		 * @param func2 function to handle an object node
		 * @param transform the node's transform
		 */
		void updateChildren(
			std::function<void(SceneObject*, gml::Matrix4D<float>)> func,
			std::function<gml::Matrix4D<float>(SceneGroup*, gml::Matrix4D<float>)> func2,
			gml::Matrix4D<float> transform);

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
