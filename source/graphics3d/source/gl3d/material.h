#pragma once

#include <map>
#include <unordered_map>
#include <string>

#include <core/gl/graphics.h>

namespace yage::gl3d
{
	/**
	 * @brief Represents a material as used by a shader program.
	 * 
	 */
	class Material
	{
	public:
		Material() = default;

		/**
		 * @brief Adds a texture to be used as a shader sampler uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param tex the texture object
		 */
		void add_uniform(const std::string& name, std::shared_ptr<gl::ITexture2D> tex);

		/**
		 * @brief Adds a Vec3f to be used as a shader vec3 uniform.
		 * 
		 * @param name the shader uniform's name
		 * @param value the value to be set
		 */
		void add_uniform(const std::string& name, math::Vec3f value);

		/**
		 * @brief Adds a float value to be used as a shader float uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param value the value to be set
		 */
		void add_uniform(const std::string& name, float value);

		/**
		 * @brief Adds an int value to be used as a shader int uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param value the value to be set
		 */
		void add_uniform(const std::string& name, int value);

		/**
		 * @brief Updates the shader's material uniforms with local values.
		 */
		void update_shader_uniforms();

		/**
		 * @brief Sets the shader to be used by this material.
		 * 
		 * @param shader the shader to set
		 */
		void set_shader(std::shared_ptr<gl::IShader> shader);

		/**
		 * @brief Returns the shader used by this material.
		 * 
		 * @return the shader
		 */
		[[nodiscard]]
		std::shared_ptr<gl::IShader> shader() const;

        /**
         * Binds the textures to the units corresponding to their uniform values.
         * @param renderer The renderer to use for binding.
         */
        void bind_textures(gl::IRenderer& renderer);

	private:
		std::shared_ptr<gl::IShader> m_shader;
		std::map<std::string, std::shared_ptr<gl::ITexture2D>> m_textures;
		std::unordered_map<std::string, math::Vec3f> m_vec3Values;
		std::unordered_map<std::string, float> m_fValues;
		std::unordered_map<std::string, int> m_iValues;
	};
}