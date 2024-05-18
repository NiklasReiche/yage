#include "sceneObject.h"

namespace gl3d
{
	SceneObject::SceneObject()
		: SceneNode(NodeType::OBJECT, "", gml::matrix::Id<double, 4>)
	{
	}

	SceneObject::SceneObject(const std::string& name, const gml::Mat4d transform)
		: SceneNode(NodeType::OBJECT, name, transform)
	{
	}

	void SceneObject::bindMesh(const std::shared_ptr<Mesh>& mesh)
	{
		this->m_mesh = mesh;
	}

	void SceneObject::bindLight(const std::shared_ptr<Light>& light)
	{
		this->m_light = light;
	}

	void SceneObject::bindCamera(const std::shared_ptr<Camera>& camera)
	{
		this->m_camera = camera;
	}

	bool SceneObject::hasMesh() const
	{
		return m_mesh != nullptr;
	}

	bool SceneObject::hasLight() const
	{
		return m_light != nullptr;
	}

	bool SceneObject::hasCamera() const
	{
		return m_camera != nullptr;
	}

	std::shared_ptr<Mesh> SceneObject::getMesh() const
	{
		return m_mesh;
	}

	std::shared_ptr<Light> SceneObject::getLight() const
	{
		return m_light;
	}

	std::shared_ptr<Camera> SceneObject::getCamera() const
	{
		return m_camera;
	}
}
