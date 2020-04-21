#pragma once

#include "light.h"

namespace gl3d
{
	class DirLight : public Light {
	private:
		gml::Vec3f direction;

	public:
		DirLight();
		explicit DirLight(gml::Vec3f direction);
		DirLight(LightColor colors, gml::Vec3f direction);

		gml::Vec3f getDirection() const;

		void setDirection(gml::Vec3f direction);
	};
}