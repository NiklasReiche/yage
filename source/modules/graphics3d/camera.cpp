#include "camera.h"

namespace graphics3d
{
	Camera::Camera() {
		this->position = gml::Vec3f(0.0f);
		this->rotation = gml::Quaternion<float>();
	}

	Camera::Camera(gml::Vec3f position, gml::Quaternion<float> rotation) {
		this->position = position;
		this->rotation = rotation;
	}

	gml::Matrix4D<float> Camera::getViewMatrix() {
		return gml::inverse<float>(gml::translate<float>(position) * gml::rotate<float>(rotation));
	}
}