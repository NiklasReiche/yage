#pragma once

#include <map>
#include <string>

#include <core/gl/graphics.h>

// TODO handling of unavailable uniforms

namespace gl3d
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
		void addTexture(const std::string& name, std::shared_ptr<gl::ITexture2D> tex);

		/**
		 * @brief Adds a Vec3f to be used as a shader vec3 uniform.
		 * 
		 * @param name the shader uniform's name
		 * @param value the value to be set
		 */
		void addVec3(const std::string& name, const gml::Vec3f& value);

		/**
		 * @brief Adds a float value to be used as a shader float uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param value the value to be set
		 */
		void addFloat(const std::string& name, float value);

		/**
		 * @brief Adds an int value to be used as a shader int uniform.
		 * 
		 * @param name the name of the shader uniform
		 * @param value the value to be set
		 */
		void addInteger(const std::string& name, int value);

		/**
		 * @brief Updates the shader's material uniforms with local values.
		 */
		void updateShader();

		/**
		 * @brief Sets the shader to be used by this material.
		 * 
		 * @param shader the shader to set
		 */
		void setShader(std::shared_ptr<gl::IShader> shader);

		/**
		 * @brief Returns the shader used by this material.
		 * 
		 * @return the shader
		 */
		[[nodiscard]]
		std::shared_ptr<gl::IShader> getShader() const;

        [[nodiscard]]
        std::vector<std::reference_wrapper<gl::ITexture2D>> textures() const;

	private:
		std::shared_ptr<gl::IShader> shader;
		std::map<std::string, std::shared_ptr<gl::ITexture2D>> m_textures;
		std::map<std::string, gml::Vec3f> vec3Values;
		std::map<std::string, float> fValues;
		std::map<std::string, int> iValues;
	};
}