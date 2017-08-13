#pragma once

#include <math/MVector.h>


namespace graphics3d
{
	class Camera
	{
	private:
		gml::Vec3f position;
		gml::Quaternion<float> rotation;

	public:
		Camera();
		Camera(gml::Vec3f position, gml::Quaternion<float> rotation);
		gml::Matrix4D<float> getViewMatrix();
	};
}