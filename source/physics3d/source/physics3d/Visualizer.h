#pragma once

#include <core/gl/Context.h>
#include <core/gl/Shader.h>
#include <core/gl/Drawable.h>
#include <core/gl/color.h>

namespace yage::physics3d
{
    /**
     * Can draw geometric primitives like points and vectors to visualize various aspects of the physics simulation.
     * Allows users to accumulate primitives to draw in subsequent frames. Primitives must be manually cleared when they
     * are no longer valid or should no longer be visualized.
     */
    class Visualizer
    {
    public:
        struct Vector
        {
            math::Vec3d support;
            math::Vec3d direction;
        };

        /**
         * Holds the point primitives to draw.
         */
        std::vector<std::tuple<math::Vec3d, gl::Color_t>> points;

        /**
         * Holds the vector primitives to draw.
         */
        std::vector<std::tuple<Vector, gl::Color_t>> vectors;

        Visualizer() = default;

        explicit Visualizer(gl::IContext& context);

        /**
         * Draws all held primitives.
         */
        void draw(const math::Mat4d& projection, const math::Mat4d& view);

    private:
        std::shared_ptr<gl::IShader> m_point_shader;
        std::shared_ptr<gl::IShader> m_vector_shader;
        std::shared_ptr<gl::IDrawable> m_empty_drawable;
        std::shared_ptr<gl::IRenderer> m_renderer;
    };
}
