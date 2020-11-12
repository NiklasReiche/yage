#pragma once

#include "math/vector.h"
#include "graphics/color.h"

namespace gl3d
{
	enum class LightType
	{
		LIGHT,
		DIR_LIGHT,
		POINT_LIGHT,
	};
	struct LightConstants {
		float constant = 0.0f;
		float linear = 0.0f;
		float quadratic = 0.0f;
	};
	struct LightColor {
		gml::Vec3f ambient;
		gml::Vec3f diffuse;
		gml::Vec3f specular;
	};

	class Light {
	private:
		const LightType type;
		LightColor color;

	protected:
		Light(LightType type, LightColor colors);

	public:
		Light();
		virtual ~Light() = default;

		LightType getType() const;
		LightColor getColors() const;

		void setColors(LightColor colors);
		void setAmbient(gml::Vec3f ambient);
		void setAmbient(uint32_t ambient);
		void setDiffuse(gml::Vec3f diffuse);
		void setDiffuse(uint32_t diffuse);
		void setSpecular(gml::Vec3f specular);
		void setSpecular(uint32_t specular);
	};

	// TODO 
	struct SpotLight : public Light {
		LightConstants constants;
		gml::Vec3d position;
		gml::Vec3d direction;
		float cutOffInner;
		float cutOffOuter;
	};
}
