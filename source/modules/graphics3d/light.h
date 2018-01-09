#pragma once

#include "math/vector.h"

namespace graphics3d
{
	struct ColorValue {
		gml::Vec3f ambient;
		gml::Vec3f diffuse;
		gml::Vec3f specular;
	};

	struct DirLight {
		ColorValue color;
		gml::Vec3f direction;
	};

	struct PointLight {
		ColorValue color;
		gml::Vec3f position;
		float constant;
		float linear;
		float quadratic;
	};
}
