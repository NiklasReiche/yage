#pragma once

#include <math/math.h>

namespace yage::gl3d
{
	class Camera
	{
	public:
		Camera();
		Camera(const math::Vec3d& position, math::Quatd rotation);
		
		void move(const math::Vec3d& vector);
		void moveTo(math::Vec3d position);
		void moveForward(double amount);
		void moveBackward(double amount);
		void moveLeft(double amount);
		void moveRight(double amount);

		void rotate(math::Quatd quaternion);
		void rotateTo(math::Quatd rotation);
		void rotateYaw(double degree);
		void rotatePitch(double degree);
		void rotateRoll(double degree);
		void lookAt(math::Vec3d position, math::Vec3d target);

		[[nodiscard]] math::Mat4d getViewMatrix() const;
		[[nodiscard]] math::Vec3d getPosition() const;
		[[nodiscard]] math::Quatd getRotation() const;

	private:
		math::Vec3d m_position;
		math::Quatd m_rotation;
	};
}
