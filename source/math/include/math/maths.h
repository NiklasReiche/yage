#pragma once

#include <cmath>
#include <numbers>

namespace yage::math
{
    namespace detail
    {
        template<std::floating_point T>
        constexpr T deg_to_rad_factor = static_cast<T>(std::numbers::pi / 180.0);

        template<std::floating_point T>
        constexpr T rad_to_deg_factor = static_cast<T>(180.0 / std::numbers::pi);
    }

    /**
     * Converts degrees to radians.
     * @param deg A value in degrees.
     * @return A value in radians.
     */
    template<std::floating_point T>
    T to_rad(const T deg)
    {
        return deg * detail::deg_to_rad_factor<T>;
    }

    /**
     * Converts radians to degrees.
     * @param rad A value in radians.
     * @return A value in degrees.
     */
    template<std::floating_point T>
    T to_deg(const T rad)
    {
        return rad * detail::rad_to_deg_factor<T>;
    }

    /**
     * Normalizes a value val from a range [min_in, max_in] to the range [min_out, max_out].
     */
    template<std::floating_point T>
    T normalize(T val, T min_in, T max_in, T min_out, T max_out)
    {
        return (val - min_in) / (max_in - min_in) * (max_out - min_out) + min_out;
    }

    /**
     * Clamps a value val to the range [min, max].
     */
    template<typename T>
    T clamp(T val, T min, T max)
    {
        return std::min(std::max(val, min), max);
    }
}
