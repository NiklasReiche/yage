#include "material.h"

#include <utility>

namespace gl3d
{
	void Material::addTexture(const std::string& name, std::shared_ptr<gl::ITexture2D> tex)
	{
		textures[name] = std::move(tex);
	}

	void Material::addVec3(const std::string& name, const gml::Vec3f& value)
	{
		vec3Values[name] = value;
	}

	void Material::addFloat(const std::string& name, float value)
	{
		fValues[name] = value;
	}

	void Material::addInteger(const std::string& name, int value)
	{
		iValues[name] = value;
	}

	void Material::updateShader()
	{
		for (const auto& uniform : vec3Values)
		{
			shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (const auto& uniform : fValues)
		{
			shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (const auto& uniform : iValues)
		{
			shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (std::pair<std::string, std::shared_ptr<gl::ITexture2D>> uniform : textures)
		{
			//TODO 
		}
	}

	void Material::setShader(std::shared_ptr<gl::IShader> _shader)
	{
		this->shader = std::move(_shader);
	}

	std::shared_ptr<gl::IShader> Material::getShader() const
	{
		return shader;
	}
}
