#include "Visualizer.h"
#include "shaders.h"
#include <core/gl/color.h>

namespace physics3d
{
    Visualizer::Visualizer(gl::IContext& context)
    {
        m_point_shader = context.getShaderCreator()->createShader(shaders::PointShader::vert,
                                                                  shaders::PointShader::frag,
                                                                  shaders::PointShader::geom);

        m_vector_shader = context.getShaderCreator()->createShader(shaders::VectorShader::vert,
                                                                   shaders::VectorShader::frag,
                                                                   shaders::VectorShader::geom);

        m_empty_drawable = context.getDrawableCreator()->createDrawable(std::vector<float>{},
                                                                        std::vector<unsigned int>{},
                                                                        {},
                                                                        gl::VertexFormat::INTERLEAVED);

        m_renderer = context.getRenderer();
    }

    void Visualizer::draw(const gml::Mat4d& projection, const gml::Mat4d& view)
    {
        if (m_renderer == nullptr) {
            return;
        }

        m_renderer->enablePointSize();

        m_point_shader->setUniform("projection", projection);
        m_point_shader->setUniform("view", view);

        m_vector_shader->setUniform("projection", projection);
        m_vector_shader->setUniform("view", view);

        m_point_shader->setUniform("size", 5.0f);
        for (const auto& [point, color] : points) {
            m_point_shader->setUniform("point", point);
            m_point_shader->setUniform("color", gl::toVec3(color));
            m_renderer->useShader(*m_point_shader);
            m_renderer->draw(*m_empty_drawable);
        }

        for (const auto& [vector, color] : vectors) {
            m_vector_shader->setUniform("support", vector.support);
            m_vector_shader->setUniform("direction", vector.direction);
            m_vector_shader->setUniform("color", gl::toVec3(color));
            m_renderer->useShader(*m_vector_shader);
            m_renderer->draw(*m_empty_drawable);
        }
    }
}