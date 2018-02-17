#pragma once

#include "resourceCache.h"
#include "material.h"
#include "mesh.h"
#include "light.h"

namespace gl3d
{
	class ResourceManager
	{
	private:
		ResourceCache<Material> materialCache;
		ResourceCache<Mesh> meshCache;
		ResourceCache<Light> lightCache;

	public:
		unsigned int add(Material material)
		{
			return materialCache.add(std::make_shared<Material>(material));
		}
		unsigned int add(Mesh mesh)
		{
			return meshCache.add(std::make_shared<Mesh>(mesh));
		}
		unsigned int add(DirLight light)
		{
			return lightCache.add(std::make_shared<DirLight>(light));
		}
		unsigned int add(PointLight light)
		{
			return lightCache.add(std::make_shared<PointLight>(light));
		}
		unsigned int add(SpotLight light)
		{
			return lightCache.add(std::make_shared<SpotLight>(light));
		}

		std::shared_ptr<Material> getMaterial(unsigned int id)
		{
			return materialCache.get(id);
		}
		std::shared_ptr<Mesh> getMesh(unsigned int id)
		{
			return meshCache.get(id);
		}
		std::shared_ptr<Light> getLight(unsigned int id)
		{
			return lightCache.get(id);
		}
	};
}