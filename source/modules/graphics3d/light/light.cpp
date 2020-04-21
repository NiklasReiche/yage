#include "light.h"

namespace gl3d
{
	Light::Light(const LightType type, const LightColor colors)
		: type(type), color(colors)
	{
	}

	Light::Light()
		: type(LightType::LIGHT)
	{
	}

	LightType Light::getType() const
	{
		return type;
	}

	LightColor Light::getColors() const
	{
		return color;
	}

	void Light::setColors(const LightColor colors)
	{
		this->color = colors;
	}

	void Light::setAmbient(const gml::Vec3f ambient)
	{
		this->color.ambient = ambient;
	}

	void Light::setAmbient(const uint32_t ambient)
	{
		this->color.ambient = gl::toVec3(ambient);
	}

	void Light::setDiffuse(const gml::Vec3f diffuse)
	{
		this->color.diffuse = diffuse;
	}

	void Light::setDiffuse(const uint32_t diffuse)
	{
		this->color.diffuse = gl::toVec3(diffuse);
	}

	void Light::setSpecular(const gml::Vec3f specular)
	{
		this->color.specular = specular;
	}

	void Light::setSpecular(const uint32_t specular)
	{
		this->color.specular = gl::toVec3(specular);
	}
}
