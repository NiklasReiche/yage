#pragma once

#include <math/MVector.h>


namespace graphics3d
{
	class Camera
	{
	private:
		

	public:
		Camera();
		Camera(gml::Vec3f position, gml::Quaternion<float> rotation);
		void update();
		gml::Matrix4D<float> getViewMatrix();

		gml::Vec3f position;
		gml::Quaternion<float> rotation;
	};
}