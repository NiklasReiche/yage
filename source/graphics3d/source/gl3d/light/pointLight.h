#pragma once

#include "light.h"

namespace gl3d
{
	class PointLight : public Light {
	private:
		LightConstants constants;
		gml::Vec3f position;

	public:
		PointLight();
		explicit PointLight(gml::Vec3f position);
		PointLight(LightColor colors, LightConstants constants, gml::Vec3f position = gml::Vec3f(0.0f));

		LightConstants getConstants() const;
		gml::Vec3f getPosition() const;

		void setConstants(LightConstants constants);
		void setPosition(gml::Vec3f position);
	};
}
