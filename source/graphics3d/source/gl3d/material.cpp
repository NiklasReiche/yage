#include "material.h"

#include <utility>

namespace gl3d
{
	void Material::add_uniform(const std::string& name, std::shared_ptr<gl::ITexture2D> tex)
	{
        m_textures[name] = std::move(tex);
	}

	void Material::add_uniform(const std::string& name, gml::Vec3f value)
	{
        m_vec3Values[name] = value;
	}

	void Material::add_uniform(const std::string& name, float value)
	{
        m_fValues[name] = value;
	}

	void Material::add_uniform(const std::string& name, int value)
	{
        m_iValues[name] = value;
	}

	void Material::update_shader_uniforms()
	{
		for (const auto& uniform : m_vec3Values)
		{
			m_shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (const auto& uniform : m_fValues)
		{
			m_shader->setUniform("material." + uniform.first, uniform.second);
		}
		for (const auto& uniform : m_iValues)
		{
			m_shader->setUniform("material." + uniform.first, uniform.second);
		}

        int i = 0;
		for (const auto& [name, _] : m_textures)
		{
            // it's important to use the same order as when binding textures to get the correct texture units
			m_shader->setUniform("material." + name, i++);
		}
	}

	void Material::set_shader(std::shared_ptr<gl::IShader> shader)
	{
		this->m_shader = std::move(shader);
	}

	std::shared_ptr<gl::IShader> Material::shader() const
	{
		return m_shader;
	}

    void Material::bind_textures(gl::IRenderer& renderer)
    {
        int i = 0;
        for (const auto& [_, texture]: m_textures) {
            renderer.bindTexture(*texture, i++);
        }
    }
}
