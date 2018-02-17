#pragma once

#include <map>

#include "sceneNode.h"

namespace gl3d
{
	/**
	 * @brief Represents different resource types.
	 * To be used as id types in a scene object.
	 */
	enum class SceneObjectIdCode
	{
		MATERIAL,
		MESH,
		LIGHT,
		CUSTOM_1,
		CUSTOM_2,
		CUSTOM_3,
		CUSTOM_4,
		CUSTOM_5
	};

	/**
	 * @brief Represents a group node in a scene graph. 
	 * This node type doesn't own any children. Different integer id values can
	 * be associated with a node to be used as pointers to objects in the 
	 * resource manager.
	 */
	class SceneObject : public SceneNode
	{
	private:
		std::map<SceneObjectIdCode, int> resourceIds;

	public:
		/**
		 * @brief Constructs an object node with an empty name.
		 */
		SceneObject();

		/**
		 * @brief Constructs an object node with a custom name.
		 * 
		 * @param name the node's name
		 */
		explicit SceneObject(const std::string name);

		/**
		 * @brief Adds an id to the given id target. 
		 * 
		 * @param code the id target
		 * @param nodeId the id to add
		 */
		void addId(SceneObjectIdCode code, int nodeId);

		/**
		 * @brief Checks whether this node has an id bound to the given target.
		 * 
		 * @param code the id target
		 * @return true if the target is bound, false otherwise
		 */
		bool hasId(SceneObjectIdCode code) const;

		/**
		 * @brief Returns the id bound to the given target.
		 * 
		 * @param code the id target
		 * @return the id
		 */
		int getId(SceneObjectIdCode code) const;
	};
}
