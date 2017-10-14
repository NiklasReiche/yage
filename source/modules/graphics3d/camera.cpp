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

	void Camera::move(gml::Vec3f vector)
	{
		this->position += vector;
	}
	void Camera::moveTo(gml::Vec3f position)
	{
		this->position = position;
	}
	void Camera::moveForward(float amount)
	{
		this->position += rotation.getForward() * amount;
	}
	void Camera::moveBackward(float amount)
	{
		this->position += rotation.getForward() * -amount;
	}
	void Camera::moveLeft(float amount)
	{
		this->position += rotation.getRight() * -amount;
	}
	void Camera::moveRight(float amount)
	{
		this->position += rotation.getRight() * amount;
	}

	void Camera::rotate(gml::Quaternion<float> quaternion)
	{
		this->rotation *= quaternion;
	}
	void Camera::rotateTo(gml::Quaternion<float> rotation)
	{
		this->rotation = rotation;
	}
	void Camera::rotateYaw(float degree)
	{
		this->rotation = this->rotation * gml::eulerAngle<float>(0, degree, 0);
		this->rotation = gml::normalize<float>(this->rotation);
	}
	void Camera::rotatePitch(float degree)
	{
		this->rotation = this->rotation * gml::eulerAngle<float>(degree, 0, 0);
		this->rotation = gml::normalize<float>(this->rotation);
	}
	void Camera::rotateRoll(float degree)
	{
		this->rotation = this->rotation * gml::eulerAngle<float>(0, 0, degree);
		this->rotation = gml::normalize<float>(this->rotation);
	}

	gml::Matrix4D<float> Camera::getViewMatrix() {
		return gml::inverse<float>(gml::translate<float>(position) * gml::rotate<float>(rotation));
	}
}