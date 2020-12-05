#pragma once

namespace physics3d
{
	class Shape
	{
	public:
		[[nodiscard]]
		double getAngularMass() const;

	private:
		double angularMass = 0;
	};

	inline double calcCubeAngularMass(double length, double mass)
	{
		return mass * length * length / 6;
	}
}
