#include "sceneObject.h"

namespace gl3d
{
	SceneObject::SceneObject()
		: SceneNode(NodeType::OBJECT, "", gml::Mat4d())
	{
	}

	SceneObject::SceneObject(const std::string name, const gml::Mat4d transform)
		: SceneNode(NodeType::OBJECT, name, transform)
	{
	}

	void SceneObject::bindMaterial(const std::shared_ptr<Material> material)
	{
		this->material = material;
	}

	void SceneObject::bindMesh(const std::shared_ptr<Mesh> mesh)
	{
		this->mesh = mesh;
	}

	void SceneObject::bindLight(const std::shared_ptr<Light> light)
	{
		this->light = light;
	}

	void SceneObject::bindCamera(const std::shared_ptr<Camera> camera)
	{
		this->camera = camera;
	}

	bool SceneObject::hasMaterial() const
	{
		return material != nullptr;
	}

	bool SceneObject::hasMesh() const
	{
		return mesh != nullptr;
	}

	bool SceneObject::hasLight() const
	{
		return light != nullptr;
	}

	bool SceneObject::hasCamera() const
	{
		return camera != nullptr;
	}

	std::shared_ptr<Material> SceneObject::getMaterial() const
	{
		return material;
	}

	std::shared_ptr<Mesh> SceneObject::getMesh() const
	{
		return mesh;
	}

	std::shared_ptr<Light> SceneObject::getLight() const
	{
		return light;
	}

	std::shared_ptr<Camera> SceneObject::getCamera() const
	{
		return camera;
	}
}
