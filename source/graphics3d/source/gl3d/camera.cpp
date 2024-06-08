#include "camera.h"

namespace gl3d
{
	Camera::Camera() {
		m_position = gml::Vec3d(0.0);
		m_rotation = gml::Quatd();
	}
	Camera::Camera(const gml::Vec3d& position, gml::Quatd rotation) {
		m_position = position;
		m_rotation = rotation;
	}

	void Camera::move(const gml::Vec3d& vector)
	{
		m_position += vector;
	}
	void Camera::moveTo(gml::Vec3d _position)
	{
		m_position = _position;
	}
	void Camera::moveForward(double amount)
	{
		m_position += m_rotation.getForward() * amount;
	}
	void Camera::moveBackward(double amount)
	{
		m_position -= m_rotation.getForward() * amount;
	}
	void Camera::moveLeft(double amount)
	{
		m_position -= m_rotation.getRight() * amount;
	}
	void Camera::moveRight(double amount)
	{
		m_position += m_rotation.getRight() * amount;
	}

	void Camera::rotate(gml::Quatd quaternion)
	{
		m_rotation *= quaternion;
	}
	void Camera::rotateTo(gml::Quatd _rotation)
	{
		m_rotation = _rotation;
	}
	void Camera::rotateYaw(double degree)
	{
		m_rotation = this->m_rotation * gml::quaternion::eulerAngle<double>(gml::to_rad(degree), 0, 0);
		m_rotation = gml::normalize(this->m_rotation);
	}
	void Camera::rotatePitch(double degree)
	{
		m_rotation = this->m_rotation * gml::quaternion::eulerAngle<double>(0, 0, gml::to_rad(-degree));
		m_rotation = gml::normalize(this->m_rotation);
	}
	void Camera::rotateRoll(double degree)
	{
		m_rotation = this->m_rotation * gml::quaternion::eulerAngle<double>(0, gml::to_rad(degree), 0);
		m_rotation = gml::normalize(this->m_rotation);
	}

	void Camera::lookAt(gml::Vec3d position, gml::Vec3d target)
	{
        // TODO
	}

	gml::Mat4d Camera::getViewMatrix() const
	{
		return gml::matrix::lookAt<double>(m_position, m_position + m_rotation.getForward(), m_rotation.getUp());
	}
	gml::Vec3d Camera::getPosition() const
	{
		return m_position;
	}
	gml::Quatd Camera::getRotation() const
	{
		return m_rotation;
	}
}
