#pragma once

#include <gml/vector.h>

namespace physics3d::collision
{
	struct Shape
	{

	};

	struct BoundingSphere
	{
		gml::Vec3d center;
		double radius;
	};

	struct BoundingBox
	{
		gml::Vec3d a;
		gml::Vec3d b;
	};

	bool checkCollision(const BoundingSphere& a, const BoundingSphere& b)
	{
		return gml::length(a.center - b.center) <= a.radius + b.radius;
	}
}
