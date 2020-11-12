#include "material.h"

namespace gl3d
{
	void Material::addTexture(std::string name, gl::Texture2D tex)
	{
		textures[name] = tex;
	}

	void Material::addVec3(std::string name, gml::Vec3f value)
	{
		vec3Values[name] = value;
	}

	void Material::addFloat(std::string name, float value)
	{
		fValues[name] = value;
	}

	void Material::addInteger(std::string name, int value)
	{
		iValues[name] = value;
	}

	void Material::updateShader()
	{
		for (std::pair<std::string, gml::Vec3f> uniform : vec3Values)
		{
			shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (std::pair<std::string, float> uniform : fValues)
		{
			shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (std::pair<std::string, int> uniform : iValues)
		{
			shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (std::pair<std::string, gl::Texture2D> uniform : textures)
		{
			//TODO 
		}
	}

	void Material::setShader(gl::Shader shader)
	{
		this->shader = shader;
	}

	gl::Shader Material::getShader() const
	{
		return shader;
	}
}
