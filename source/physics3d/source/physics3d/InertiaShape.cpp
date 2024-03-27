#include "InertiaShape.h"

namespace physics3d
{
    CubeShape::CubeShape(double length, double mass)
    {
        this->mass = mass;
        double moment = mass * length * length / 6;
        this->inertiaTensor = {
                moment, 0, 0,
                0, moment, 0,
                0, 0, moment
        };
        this->inverseInertiaTensor = gml::inverse(this->inertiaTensor);
    }

    BoxShape::BoxShape(double width, double height, double depth, double mass)
    {
        this->mass = mass;
        this->inertiaTensor = {
                mass * (height * height + depth * depth) / 12, 0, 0,
                0, mass * (width * width + height * height) / 12, 0,
                0, 0, mass * (width * width + depth * depth) / 12
        };
        this->inverseInertiaTensor = gml::inverse(this->inertiaTensor);
    }

    SphereShape::SphereShape(double radius, double mass)
    {
        this->mass = mass;
        double moment = mass * radius * radius * 2 / 5;
        this->inertiaTensor = {
                moment, 0, 0,
                0, moment, 0,
                0, 0, moment
        };
        this->inverseInertiaTensor = gml::inverse(this->inertiaTensor);
    }
}
