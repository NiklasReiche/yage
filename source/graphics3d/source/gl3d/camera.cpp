#include "camera.h"

namespace yage::gl3d
{
	Camera::Camera() {
		m_position = math::Vec3d(0.0);
		m_rotation = math::Quatd();
	}

	Camera::Camera(const math::Vec3d& position, const math::Quatd& rotation) {
		m_position = position;
		m_rotation = rotation;
	}

	void Camera::move_by(const math::Vec3d& vector)
	{
		m_position += vector;
	}

	void Camera::move_to(const math::Vec3d& position)
	{
		m_position = position;
	}

	void Camera::move_forward(const double amount)
	{
		m_position += m_rotation.forward_direction() * amount;
	}

	void Camera::move_backward(const double amount)
	{
		m_position -= m_rotation.forward_direction() * amount;
	}

	void Camera::move_left(const double amount)
	{
		m_position -= m_rotation.right_direction() * amount;
	}

	void Camera::move_right(const double amount)
	{
		m_position += m_rotation.right_direction() * amount;
	}

	void Camera::rotate_by(const math::Quatd& quaternion)
	{
		m_rotation *= quaternion;
	}

	void Camera::rotate_to(const math::Quatd& rotation)
	{
		m_rotation = rotation;
	}

	void Camera::rotate_yaw(const double degree)
	{
		m_rotation = m_rotation * math::quaternion::euler_angle<double>(math::to_rad(degree), 0, 0);
		m_rotation = normalize(m_rotation);
	}

	void Camera::rotate_pitch(const double degree)
	{
		m_rotation = m_rotation * math::quaternion::euler_angle<double>(0, 0, math::to_rad(-degree));
		m_rotation = normalize(m_rotation);
	}

	void Camera::rotate_roll(const double degree)
	{
		m_rotation = m_rotation * math::quaternion::euler_angle<double>(0, math::to_rad(degree), 0);
		m_rotation = normalize(m_rotation);
	}

	void Camera::look_at(const math::Vec3d& position, const math::Vec3d& target)
	{
		auto direction = normalize(target - position);
		auto right = normalize(cross(math::vector::worldUp<double>(), direction));
		auto up = normalize(cross(direction, right));
		const math::Mat3d mat(
			right.x(), up.x(), direction.x(),
			right.y(), up.y(), direction.y(),
			right.z(), up.z(), direction.z()
		);

		m_rotation = math::quaternion::from_matrix(mat);
		m_position = position;
	}

	math::Mat4d Camera::view_matrix() const
	{
		return math::matrix::look_at(m_position, m_position + m_rotation.forward_direction(), m_rotation.up_direction());
	}

	math::Vec3d Camera::position() const
	{
		return m_position;
	}

	math::Quatd Camera::rotation() const
	{
		return m_rotation;
	}
}
