#include "dirLight.h"

namespace gl3d
{
	DirLight::DirLight()
		: Light(LightType::DIR_LIGHT, LightColor())
	{
	}

	DirLight::DirLight(const gml::Vec3f direction)
		: Light(LightType::DIR_LIGHT, LightColor()), direction(direction)
	{
	}

	DirLight::DirLight(const LightColor colors, const gml::Vec3f direction)
		: Light(LightType::DIR_LIGHT, colors), direction(direction)
	{
	}

	gml::Vec3f DirLight::getDirection() const
	{
		return direction;
	}

	void DirLight::setDirection(const gml::Vec3f direction)
	{
		this->direction = direction;
	}
}
