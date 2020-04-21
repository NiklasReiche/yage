#pragma once

#include <map>

#include "sceneNode.h"
#include "../material.h"
#include "../mesh.h"
#include "../camera.h"
#include "../light/light.h"

namespace gl3d
{
	/**
	 * @brief Represents a group node in a scene graph. 
	 * This node type doesn't own any children. Different integer id values can
	 * be associated with a node to be used as pointers to objects in the 
	 * resource manager.
	 */
	class SceneObject : public SceneNode
	{
	private:
		std::shared_ptr<Material> material = nullptr;
		std::shared_ptr<Mesh> mesh = nullptr;
		std::shared_ptr<Light> light = nullptr;
		std::shared_ptr<Camera> camera = nullptr;

	public:
		/**
		 * @brief Constructs an object node with an empty name.
		 */
		SceneObject();

		/**
		 * @brief Constructs an object node with a custom name and transform.
		 * 
		 * @param name the node's name
		 * @param transform the node's local transform
		 */
		explicit SceneObject(
			std::string name,
			gml::Mat4d transform = gml::Mat4d());


		void bindMaterial(std::shared_ptr<Material> material);

		void bindMesh(std::shared_ptr<Mesh> mesh);

		void bindLight(std::shared_ptr<Light> light);

		void bindCamera(std::shared_ptr<Camera> camera);

		
		bool hasMaterial() const;

		bool hasMesh() const;

		bool hasLight() const;

		bool hasCamera() const;


		std::shared_ptr<Material> getMaterial() const;

		std::shared_ptr<Mesh> getMesh() const;

		std::shared_ptr<Light> getLight() const;

		std::shared_ptr<Camera> getCamera() const;
	};
}
