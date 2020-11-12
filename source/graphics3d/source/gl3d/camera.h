#pragma once

#include <gml/gml.h>

namespace gl3d
{
	class Camera
	{
	public:
		Camera();
		Camera(const gml::Vec3d& position, gml::Quatd rotation);
		
		void move(const gml::Vec3d& vector);
		void moveTo(gml::Vec3d position);
		void moveForward(double amount);
		void moveBackward(double amount);
		void moveLeft(double amount);
		void moveRight(double amount);

		void rotate(gml::Quatd quaternion);
		void rotateTo(gml::Quatd rotation);
		void rotateYaw(double degree);
		void rotatePitch(double degree);
		void rotateRoll(double degree);
		void lookAt(gml::Vec3d target, double degree = 0.0f);

		[[nodiscard]] gml::Mat4d getViewMatrix() const;
		[[nodiscard]] gml::Vec3d getPosition() const;
		[[nodiscard]] gml::Quatd getRotation() const;

	private:
		gml::Vec3d position;
		gml::Quatd rotation;
	};
}
