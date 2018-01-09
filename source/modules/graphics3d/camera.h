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
		
		void move(gml::Vec3f move);
		void moveTo(gml::Vec3f position);
		void moveForward(float amount);
		void moveBackward(float amount);
		void moveLeft(float amount);
		void moveRight(float amount);

		void rotate(gml::Quaternion<float> rotate);
		void rotateTo(gml::Quaternion<float> rotation);
		void rotateYaw(float degree);
		void rotatePitch(float degree);
		void rotateRoll(float degree);

		gml::Matrix4D<float> getViewMatrix();
		gml::Vec3f getPosition();
	};
}