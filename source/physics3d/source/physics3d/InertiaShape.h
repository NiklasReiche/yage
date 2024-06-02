#pragma once

#include <gml/matrix.h>

namespace physics3d
{
    /**
     * Encodes the shape of a rigid body with respect to its mass distribution. Describes the mass and inertia of an
     * object.
     */
    class InertiaShape
    {
    public:
        [[nodiscard]] double inverse_mass() const;

        [[nodiscard]] gml::Mat3d inverse_inertia_tensor() const;

        /**
         * @return An immovable static shape.
         */
        static InertiaShape static_shape();

        /**
         * @return A spherical shape of uniform mass.
         */
        static InertiaShape sphere(double radius, double mass);

        /**
         * @return A cube of uniform mass.
         */
        static InertiaShape cube(double length, double mass);

        /**
         * @return A cuboid of uniform mass.
         */
        static InertiaShape cuboid(double width, double height, double depth, double mass);

    private:
        InertiaShape(double inverse_mass, gml::Mat3d inverse_inertia_tensor);

        double m_inverse_mass = 1;
        gml::Mat3d m_inverse_inertia_tensor;
    };
}
