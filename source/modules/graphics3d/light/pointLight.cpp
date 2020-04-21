#include "pointLight.h"

namespace gl3d
{
	PointLight::PointLight()
		: Light(LightType::POINT_LIGHT, LightColor())
	{
	}

	PointLight::PointLight(const gml::Vec3f position)
		:Light(LightType::POINT_LIGHT, LightColor()), position(position)
	{
	}

	PointLight::PointLight(const LightColor colors, const LightConstants constants, const gml::Vec3f position)
		:Light(LightType::POINT_LIGHT, colors), constants(constants), position(position)
	{
	}

	LightConstants PointLight::getConstants() const
	{
		return constants;
	}

	gml::Vec3f PointLight::getPosition() const
	{
		return position;
	}

	void PointLight::setConstants(const LightConstants constants)
	{
		this->constants = constants;
	}

	void PointLight::setPosition(const gml::Vec3f position)
	{
		this->position = position;
	}
}
