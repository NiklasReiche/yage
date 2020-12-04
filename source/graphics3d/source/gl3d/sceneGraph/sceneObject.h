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
	 * @brief Represents a leaf node in a scene graph.
	 * This node type doesn't own any children.
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
		explicit SceneObject(const std::string& name, gml::Mat4d transform = gml::matrix::Id<double, 4>);


		void bindMaterial(const std::shared_ptr<Material>& material);

		void bindMesh(const std::shared_ptr<Mesh>& mesh);

		void bindLight(const std::shared_ptr<Light>& light);

		void bindCamera(const std::shared_ptr<Camera>& camera);

		
		[[nodiscard]] bool hasMaterial() const;

		[[nodiscard]] bool hasMesh() const;

		[[nodiscard]] bool hasLight() const;

		[[nodiscard]] bool hasCamera() const;


		[[nodiscard]] std::shared_ptr<Material> getMaterial() const;

		[[nodiscard]] std::shared_ptr<Mesh> getMesh() const;

		[[nodiscard]] std::shared_ptr<Light> getLight() const;

		[[nodiscard]] std::shared_ptr<Camera> getCamera() const;
	};
}
