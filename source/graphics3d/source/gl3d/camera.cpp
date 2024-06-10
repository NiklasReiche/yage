#include "camera.h"

namespace yage::gl3d
{
	Camera::Camera() {
		m_position = math::Vec3d(0.0);
		m_rotation = math::Quatd();
	}
	Camera::Camera(const math::Vec3d& position, math::Quatd rotation) {
		m_position = position;
		m_rotation = rotation;
	}

	void Camera::move(const math::Vec3d& vector)
	{
		m_position += vector;
	}
	void Camera::moveTo(math::Vec3d _position)
	{
		m_position = _position;
	}
	void Camera::moveForward(double amount)
	{
		m_position += m_rotation.forward_direction() * amount;
	}
	void Camera::moveBackward(double amount)
	{
		m_position -= m_rotation.forward_direction() * amount;
	}
	void Camera::moveLeft(double amount)
	{
		m_position -= m_rotation.right_direction() * amount;
	}
	void Camera::moveRight(double amount)
	{
		m_position += m_rotation.right_direction() * amount;
	}

	void Camera::rotate(math::Quatd quaternion)
	{
		m_rotation *= quaternion;
	}
	void Camera::rotateTo(math::Quatd _rotation)
	{
		m_rotation = _rotation;
	}
	void Camera::rotateYaw(double degree)
	{
		m_rotation = this->m_rotation * math::quaternion::euler_angle<double>(math::to_rad(degree), 0, 0);
		m_rotation = math::normalize(this->m_rotation);
	}
	void Camera::rotatePitch(double degree)
	{
		m_rotation = this->m_rotation * math::quaternion::euler_angle<double>(0, 0, math::to_rad(-degree));
		m_rotation = math::normalize(this->m_rotation);
	}
	void Camera::rotateRoll(double degree)
	{
		m_rotation = this->m_rotation * math::quaternion::euler_angle<double>(0, math::to_rad(degree), 0);
		m_rotation = math::normalize(this->m_rotation);
	}

	void Camera::lookAt(math::Vec3d position, math::Vec3d target)
	{
        // TODO
	}

	math::Mat4d Camera::getViewMatrix() const
	{
		return math::matrix::look_at<double>(m_position, m_position + m_rotation.forward_direction(), m_rotation.up_direction());
	}
	math::Vec3d Camera::getPosition() const
	{
		return m_position;
	}
	math::Quatd Camera::getRotation() const
	{
		return m_rotation;
	}
}
