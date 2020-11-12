#include "camera.h"

namespace gl3d
{
	Camera::Camera() {
		this->position = gml::Vec3d(0.0);
		this->rotation = gml::Quatd();
	}
	Camera::Camera(const gml::Vec3d& position, gml::Quatd rotation) {
		this->position = position;
		this->rotation = rotation;
	}

	void Camera::move(const gml::Vec3d& vector)
	{
		this->position += vector;
	}
	void Camera::moveTo(gml::Vec3d _position)
	{
		this->position = _position;
	}
	void Camera::moveForward(double amount)
	{
		this->position += rotation.getForward() * amount;
	}
	void Camera::moveBackward(double amount)
	{
		this->position -= rotation.getForward() * amount;
	}
	void Camera::moveLeft(double amount)
	{
		this->position -= rotation.getRight() * amount;
	}
	void Camera::moveRight(double amount)
	{
		this->position += rotation.getRight() * amount;
	}

	void Camera::rotate(gml::Quatd quaternion)
	{
		this->rotation *= quaternion;
	}
	void Camera::rotateTo(gml::Quatd _rotation)
	{
		this->rotation = _rotation;
	}
	void Camera::rotateYaw(double degree)
	{
		this->rotation = this->rotation * gml::quaternion::eulerAngle<double>(gml::toRad(degree), 0, 0);
		this->rotation = gml::normalize(this->rotation);
	}
	void Camera::rotatePitch(double degree)
	{
		this->rotation = this->rotation * gml::quaternion::eulerAngle<double>(0, 0, gml::toRad(-degree));
		this->rotation = gml::normalize(this->rotation);
	}
	void Camera::rotateRoll(double degree)
	{
		this->rotation = this->rotation * gml::quaternion::eulerAngle<double>(0, gml::toRad(degree), 0);
		this->rotation = gml::normalize(this->rotation);
	}

	void Camera::lookAt(gml::Vec3d , double )
	{
		// TODO
	}

	gml::Mat4d Camera::getViewMatrix() const
	{
		return gml::matrix::lookAt<double>(position, position + rotation.getForward(), rotation.getUp());
	}
	gml::Vec3d Camera::getPosition() const
	{
		return position;
	}
	gml::Quatd Camera::getRotation() const
	{
		return rotation;
	}
}
